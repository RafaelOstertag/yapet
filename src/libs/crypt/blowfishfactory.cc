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

#include "blowfishfactory.hh"

using namespace yapet;

BlowfishFactory::BlowfishFactory(const SecureArray& password,
                                 const MetaData& keyingParameters)
    : _key448{new Key448{}} {
    _key448->keyingParameters(keyingParameters);
    _key448->password(password);
}

std::shared_ptr<AbstractCryptoFactory> BlowfishFactory::newFactory(
    const SecureArray& password, const MetaData& keyingParameters) const {
    return std::shared_ptr<AbstractCryptoFactory>{
        new BlowfishFactory(password, keyingParameters)};
}

std::unique_ptr<Crypto> BlowfishFactory::crypto() const {
    return std::unique_ptr<Crypto>{new Blowfish{_key448}};
}

std::unique_ptr<YapetFile> BlowfishFactory::file(const std::string& filename,
                                                 bool create,
                                                 bool secure) const {
    return std::unique_ptr<YapetFile>{
        new Yapet10File{filename, create, secure}};
}