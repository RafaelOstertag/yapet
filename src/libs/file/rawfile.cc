/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cstdio>
#include <stdexcept>

#include "consts.h"
#include "fileerror.hh"
#include "intl.h"
#include "logger.hh"
#include "ods.hh"
#include "rawfile.hh"

using namespace yapet;

const auto FILE_NOT_OPEN{_("File not open")};
const auto FILE_ALREADY_OPEN{_("File already open")};
constexpr auto READ_WRITE_EXISTING_MODE{"r+"};
constexpr auto CREATE_NEW_MODE{"w+"};
constexpr auto ONE_ITEM{1};

namespace {

inline void throwIfFileAlreadyOpen(bool alreadyOpen) {
    if (alreadyOpen) {
        throw FileError{FILE_ALREADY_OPEN};
    }
}

inline void throwIfFileNotOpen(bool fileOpen) {
    if (!fileOpen) {
        throw FileError{FILE_NOT_OPEN};
    }
}
}  // namespace

RawFile::RawFile(const std::string& filename) noexcept
    : _filename{filename}, _file{nullptr}, _openFlag{false} {}

RawFile::~RawFile() { close(); }

RawFile::RawFile(RawFile&& other)
    : _filename{std::move(other._filename)},
      _file{other._file},
      _openFlag{other._openFlag} {
    other._file = nullptr;
    other._openFlag = false;
}

RawFile& RawFile::operator=(RawFile&& other) {
    if (this == &other) {
        return *this;
    }

    close();

    _filename = std::move(other._filename);

    _file = other._file;
    other._file = nullptr;

    _openFlag = other._openFlag;
    other._openFlag = false;

    return *this;
}

void RawFile::openExisting() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), READ_WRITE_EXISTING_MODE);
    if (_file == nullptr) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open existing file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }
    _openFlag = true;
}

void RawFile::openNew() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), CREATE_NEW_MODE);
    if (_file == nullptr) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot create new file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }
    _openFlag = true;
}

std::pair<SecureArray, bool> RawFile::read(std::uint32_t size) {
    throwIfFileNotOpen(_openFlag);

    if (size < 1) {
        throw FileError(_("Read size must not be less than 1"));
    }

    SecureArray array(size);

    auto res = std::fread(*array, size, ONE_ITEM, _file);
    if (std::feof(_file)) {
        return std::pair<SecureArray, bool>{SecureArray{1}, false};
    }
    if (std::ferror(_file) || res != ONE_ITEM) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error reading file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }

    return std::pair<SecureArray, bool>{array, true};
}

std::pair<SecureArray, bool> RawFile::read() {
    throwIfFileNotOpen(_openFlag);

    record_size_type odsRecordSize;

    auto res =
        std::fread(&odsRecordSize, sizeof(record_size_type), ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file)) {
        return std::pair<SecureArray, bool>{SecureArray{1}, false};
    }

    if (std::ferror(_file)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error reading record length from file '%s'"),
                      _filename.c_str());
        throw FileError{msg, errno};
    }

    auto hostRecordSize = toHost(odsRecordSize);

    return read(hostRecordSize);
}

void RawFile::write(const SecureArray& secureArray) {
    throwIfFileNotOpen(_openFlag);

    record_size_type hostRecordSize = secureArray.size();
    auto odsRecordSize = toODS(hostRecordSize);

    auto res =
        std::fwrite(&odsRecordSize, sizeof(record_size_type), ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file) || std::ferror(_file)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot write record size to file '%s'"),
                      _filename.c_str());
        throw FileError{msg, errno};
    }

    write(*secureArray, hostRecordSize);
}

void RawFile::write(const std::uint8_t* buffer, std::uint32_t size) {
    throwIfFileNotOpen(_openFlag);

    auto res = std::fwrite(buffer, size, ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file) || std::ferror(_file)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot write buffer to file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }
}

void RawFile::rewind() { seekAbsolute(0); }

void RawFile::seekAbsolute(seek_type position) {
    throwIfFileNotOpen(_openFlag);

    if (position < 0) {
        throw std::invalid_argument{_("Position must be positive")};
    }

    auto error = std::fseek(_file, position, SEEK_SET);
    if (error || std::feof(_file) || std::ferror(_file)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error seeking in file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }
}

void RawFile::close() {
    if (_file) {
        LOG_MESSAGE(std::string{__func__} + ": " + _filename);
        std::fclose(_file);
        _file = nullptr;
    }
    _openFlag = false;
}

void RawFile::reopen() {
    throwIfFileNotOpen(_openFlag);

    _file = ::freopen(_filename.c_str(), READ_WRITE_EXISTING_MODE, _file);
    if (_file == nullptr) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error re-opening file '%s'"), _filename.c_str());
        throw FileError{msg, errno};
    }
}

RawFile::seek_type RawFile::getPosition() {
    auto currentPosition = ::ftell(_file);
    if (currentPosition == -1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Unable to get current position in file '%s'"),
                      _filename.c_str());
        throw FileError{msg, errno};
    }

    return currentPosition;
}

void RawFile::flush() {
    auto error{fflush(_file)};
    if (error) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Unable to flush data to file '%s'"),
                      _filename.c_str());
        throw FileError{msg, errno};
    }
}