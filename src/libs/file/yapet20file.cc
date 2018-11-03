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

#include <unistd.h>
#include <exception>

#include "consts.h"
#include "fileerror.hh"
#include "logger.hh"
#include "yapet20file.hh"

using namespace yapet;

constexpr std::uint8_t Yapet20File::RECOGNITION_STRING[];

Yapet20File::Yapet20File(const std::string& filename, bool create, bool secure)
    : Yapet10File{filename, create, secure} {}

Yapet20File::Yapet20File(Yapet20File&& other) : Yapet10File{std::move(other)} {}

Yapet20File& Yapet20File::operator=(Yapet20File&& other) {
    if (&other == this) {
        return *this;
    }

    YapetFile::operator=(std::move(other));

    return *this;
}

Yapet20File::~Yapet20File() {}

SecureArray Yapet20File::readUnencryptedMetaData() {
    RawFile& rawFile{getRawFile()};
    // Skip the recognition string.
    rawFile.seekAbsolute(recognitionStringSize());

    auto resultPair{rawFile.read()};
    if (resultPair.second == false) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot read unencrypted meta data from file '%s'"),
                      rawFile.filename().c_str());
        throw FileFormatError{msg};
    }

    LOG_MESSAGE(std::string{__func__} + ": " + getRawFile().filename());
    return resultPair.first;
}

void Yapet20File::writeUnencryptedMetaData(const SecureArray& metaData) {
    RawFile& rawFile{getRawFile()};

    rawFile.seekAbsolute(recognitionStringSize());

    rawFile.write(metaData);
    rawFile.flush();
    LOG_MESSAGE(std::string{__func__} + ": " + getRawFile().filename());
}

const std::uint8_t* Yapet20File::recognitionString() const {
    return Yapet20File::RECOGNITION_STRING;
}

int Yapet20File::recognitionStringSize() const {
    return Yapet20File::RECOGNITION_STRING_SIZE;
}