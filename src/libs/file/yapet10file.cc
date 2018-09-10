//
// Copyright (C) 2018 Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

/*
 * File:   Yapet10File.cc
 * Author: rafi
 *
 * Created on August 19, 2018, 3:12 PM
 */

#include <unistd.h>
#include <cstring>
#include <exception>

#include "fileerror.hh"
#include "intl.h"
#include "yapet10file.hh"

using namespace yapet;

constexpr std::uint8_t Yapet10File::RECOGNITION_STRING[];

std::string Yapet10File::recognitionStringAsString() const {
    auto recogStringSize{recognitionStringSize()};

    char* buffer{new char[recogStringSize + 1]};
    std::memcpy(buffer, reinterpret_cast<const char*>(recognitionString()),
                recogStringSize);

    buffer[recogStringSize] = '\0';
    std::string result{buffer};
    delete[] buffer;

    return result;
}

Yapet10File::Yapet10File(const std::string& filename, bool create, bool secure)
    : YapetFile{filename, create, secure} {}

Yapet10File::Yapet10File(Yapet10File&& other) : YapetFile{std::move(other)} {}

Yapet10File& Yapet10File::operator=(Yapet10File&& other) {
    if (&other == this) {
        return *this;
    }

    YapetFile::operator=(std::move(other));

    return *this;
}

Yapet10File::~Yapet10File() {}

void Yapet10File::open() {
    openRawFile();
    if (!isCreate() && !hasValidFormat()) {
        std::string message{_("File ")};
        message += filename();
        message += _(" is not a ");
        message += recognitionStringAsString();
        message += _(" file");
        throw FileFormatError(message.c_str());
    }
}

bool Yapet10File::hasValidFormat() {
    SecureArray identifier;
    try {
        identifier = readIdentifier();
    } catch (std::exception& e) {
        return false;
    }
    auto recognitionStringSizeVar{recognitionStringSize()};

    if (identifier.size() != recognitionStringSizeVar) {
        return false;
    }

    auto recognitionStringPtr{recognitionString()};
    for (auto i{0}; i < recognitionStringSizeVar; i++) {
        if (recognitionStringPtr[i] != (*identifier)[i]) {
            return false;
        }
    }

    return true;
}

SecureArray Yapet10File::readIdentifier() {
    RawFile& rawFile{getRawFile()};

    rawFile.rewind();
    auto result = rawFile.read(recognitionStringSize());
    if (result.second == false) {
        throw FileFormatError{_("Cannot read recognition string")};
    }

    return result.first;
}

SecureArray Yapet10File::readMetaData() {
    RawFile& rawFile{getRawFile()};

    // Skip the recognition string.
    rawFile.seekAbsolute(recognitionStringSize());
    auto resultPair{rawFile.read()};
    if (resultPair.second == false) {
        throw FileFormatError{_("Cannot read meta data")};
    }

    return resultPair.first;
}

std::list<SecureArray> Yapet10File::readPasswordRecords() {
    // This read is expected to leave the file position indicatory pointing to
    // the first password record length indicator
    readMetaData();

    std::list<SecureArray> passwordRecords;
    RawFile& rawFile{getRawFile()};
    std::pair<SecureArray, bool> resultPair;
    while ((resultPair = rawFile.read()).second != false) {
        passwordRecords.push_back(std::move(resultPair.first));
    }

    return passwordRecords;
}

void Yapet10File::writeIdentifier() {
    RawFile& rawFile{getRawFile()};
    rawFile.rewind();
    rawFile.write(recognitionString(), recognitionStringSize());
}

void Yapet10File::writeMetaData(const SecureArray& metaData) {
    RawFile& rawFile{getRawFile()};

    rawFile.seekAbsolute(recognitionStringSize());

    rawFile.write(metaData);
    rawFile.flush();
}

void Yapet10File::writePasswordRecords(
    const std::list<SecureArray>& passwords) {
    // This will position the file pointer on to the size indicator of the first
    // password record
    readMetaData();

    RawFile& rawFile{getRawFile()};
    RawFile::seek_type trimSize = rawFile.getPosition();

    rawFile.close();
    auto error = ::truncate(rawFile.filename().c_str(), trimSize);
    if (error) {
        throw yapet::FileError(_("Error truncating file"), errno);
    }

    rawFile.openExisting();
    rawFile.seekAbsolute(trimSize);

    for (auto password : passwords) {
        rawFile.write(password);
    }

    rawFile.flush();
}

const std::uint8_t* Yapet10File::recognitionString() const {
    return Yapet10File::RECOGNITION_STRING;
};

int Yapet10File::recognitionStringSize() const {
    return Yapet10File::RECOGNITION_STRING_SIZE;
}