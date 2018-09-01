// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#include "intl.h"

#include "blowfish.h"

using namespace YAPET;

/**
 * Initializes the class with the given key, which is used for
 * encryption and decryption.
 *
 * The constructor tries to set the key length of the cipher used to
 * the length of the key provided. If this fails, a \c YAPETException is
 * thrown.
 *
 * @param k the key used for encryption/decryption.
 *
 * @throw YAPETException in case the key length of the cipher cannot be
 * set to the length of the key provided.
 */
Blowfish::Blowfish(const std::shared_ptr<yapet::Key>& key)
    : yapet::Crypto(key) {}

Blowfish::Blowfish(const Blowfish& c) : yapet::Crypto(c) {}

Blowfish& Blowfish::operator=(const Blowfish& c) {
    if (this == &c) return *this;

    yapet::Crypto::operator=(c);

    return *this;
}

Blowfish::Blowfish(Blowfish&& c) : yapet::Crypto{c} {}

Blowfish& Blowfish::operator=(Blowfish&& c) {
    if (this == &c) return *this;

    yapet::Crypto::operator=(c);

    return *this;
}