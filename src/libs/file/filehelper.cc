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

#include <cstdio>

#include "consts.h"
#include "fileerror.hh"
#include "filehelper.hh"
#include "yapet10file.hh"
#include "yapet20file.hh"

using namespace yapet;

SecureArray yapet::readFileIdentifier(const std::string& filename) {
    try {
        Yapet10File file{filename, false, false};
        file.open();
        return file.readIdentifier();
    } catch (...) {
        // try next file type
    }

    try {
        Yapet20File file{filename, false, false};
        file.open();
        return file.readIdentifier();
    } catch (...) {
        // handled below
    }

    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
    std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                  _("'%s' is not a known file type"), filename.c_str());
    throw FileFormatError(msg);
}

bool yapet::isFileType(const SecureArray& expected,
                       const std::string& filename) {
    auto identifier{readFileIdentifier(filename)};
    return identifier == expected;
}

std::shared_ptr<YapetFile> yapet::getFile(const std::string& filename,
                                          bool secure) {
    SecureArray yapet10Identifier{toSecureArray(
        Yapet10File::RECOGNITION_STRING, Yapet10File::RECOGNITION_STRING_SIZE)};
    if (isFileType(yapet10Identifier, filename)) {
        return std::shared_ptr<YapetFile>{
            new Yapet10File{filename, false, secure}};
    }

    SecureArray yapet20Identifier{toSecureArray(
        Yapet20File::RECOGNITION_STRING, Yapet20File::RECOGNITION_STRING_SIZE)};
    if (isFileType(yapet20Identifier, filename)) {
        return std::shared_ptr<YapetFile>{
            new Yapet20File{filename, false, secure}};
    }

    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
    std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                  _("'%s' is not a known file type"), filename.c_str());
    throw FileFormatError(msg);
}

MetaData yapet::readMetaData(const std::string& filename, bool secure) {
    auto yapetFile{getFile(filename, secure)};
    yapetFile->open();
    return yapetFile->readUnencryptedMetaData();
}