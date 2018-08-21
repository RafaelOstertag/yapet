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

#include <exception>

#include "fileerror.hh"
#include "intl.h"
#include "yapet10file.hh"

using namespace yapet;

const std::uint8_t Yapet10File::_recognitionString[]{'Y', 'A', 'P', 'E',
                                                     'T', '1', '.', '0'};
const int Yapet10File::_recognitionStringSize{
    sizeof(Yapet10File::_recognitionString)};

const std::uint8_t* Yapet10File::getRecognitionString() {
    return Yapet10File::_recognitionString;
}

Yapet10File::Yapet10File(const std::string& filename, bool create, bool secure)
    : YapetFile{filename, create, secure} {
    openRawFile();
}

Yapet10File::Yapet10File(Yapet10File&& other) : YapetFile{std::move(other)} {}

Yapet10File& Yapet10File::operator=(Yapet10File&& other) {
    if (&other == this) {
        return *this;
    }

    YapetFile::operator=(std::move(other));

    return *this;
}

Yapet10File::~Yapet10File() {}

bool Yapet10File::hasValidFormat() {
    SecureArray identifier;
    try {
        identifier = readIdentifier();
    } catch (std::exception& e) {
        return false;
    }

    if (identifier.size() != Yapet10File::_recognitionStringSize) {
        return false;
    }

    for (auto i{0}; i < Yapet10File::_recognitionStringSize; i++) {
        if (Yapet10File::_recognitionString[i] != (*identifier)[i]) {
            return false;
        }
    }

    return true;
}

SecureArray Yapet10File::readIdentifier() {
    RawFile& rawFile{getRawFile()};

    rawFile.rewind();
    auto result = rawFile.read(Yapet10File::_recognitionStringSize);
    if (result.second == false) {
        throw FileFormatError{_("Cannot read recognition string")};
    }

    return result.first;
}

SecureArray Yapet10File::readMetaData() {
    RawFile& rawFile{getRawFile()};

    // Skip the recognition string.
    rawFile.seekAbsolute(Yapet10File::_recognitionStringSize);
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