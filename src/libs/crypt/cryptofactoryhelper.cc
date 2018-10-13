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

#include "cryptofactoryhelper.hh"
#include "aes256factory.hh"
#include "blowfishfactory.hh"
#include "fileerror.hh"
#include "filehelper.hh"
#include "intl.h"

using namespace yapet;

namespace {
bool isYapet10File(const std::string& filename) {
    SecureArray expectedIdentifier{toSecureArray(
        Yapet10File::RECOGNITION_STRING, Yapet10File::RECOGNITION_STRING_SIZE)};

    return isFileType(expectedIdentifier, filename);
}

bool isYapet20File(const std::string& filename) {
    SecureArray expectedIdentifier{toSecureArray(
        Yapet20File::RECOGNITION_STRING, Yapet20File::RECOGNITION_STRING_SIZE)};

    return isFileType(expectedIdentifier, filename);
}
}  // namespace

std::shared_ptr<AbstractCryptoFactory> yapet::getCryptoFactoryForFile(
    const std::string& filename, const SecureArray& password) {
    try {
        if (isYapet10File(filename)) {
            return std::shared_ptr<AbstractCryptoFactory>{
                new BlowfishFactory{password, MetaData{}}};
        }
    } catch (std::exception&) {
        // Ok, not a Yapet file
    }

    try {
        if (isYapet20File(filename)) {
            auto metaData{readMetaData(filename, false)};
            return std::shared_ptr<AbstractCryptoFactory>{
                new Aes256Factory{password, metaData}};
        }
    } catch (std::exception&) {
        // Ok, not a Yapet file
    }

    return std::shared_ptr<AbstractCryptoFactory>{};
}