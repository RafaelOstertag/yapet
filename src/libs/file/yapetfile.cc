//
// Copyright (C) 2018 Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
 * File:   YapetFile.cc
 * Author: rafi
 *
 * Created on August 19, 2018, 6:27 PM
 */

#include <cstdio>

#include "consts.h"
#include "fileerror.hh"
#include "fileutils.hh"
#include "intl.h"
#include "yapetfile.hh"

using namespace yapet;

YapetFile::YapetFile(const std::string& filename, bool create, bool secure)
    : _rawFile{filename}, _create{create}, _secure{secure} {}

YapetFile::YapetFile(YapetFile&& other)
    : _rawFile{std::move(other._rawFile)},
      _create{other._create},
      _secure{other._secure} {}

YapetFile& YapetFile::operator=(YapetFile&& other) {
    if (&other == this) {
        return *this;
    }

    _rawFile = std::move(other._rawFile);
    _create = other._create;
    _secure = other._secure;

    return *this;
}

YapetFile::~YapetFile() {}

void YapetFile::openRawFile() {
    if (_create) {
        _rawFile.openNew();
        setSecurePermissionsAndOwner(_rawFile.filename());
    } else {
        if (_secure && !hasSecurePermissions(_rawFile.filename())) {
            char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
            std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                          _("'%s' has insecure permissions"),
                          _rawFile.filename().c_str());
            throw FileInsecureError{msg};
        }

        _rawFile.openExisting();
    }
}