// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2018  Rafael Ostertag
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

#ifndef _KEY448_HH
#define _KEY448_HH 1

#include <typeinfo>

#include <openssl/evp.h>

#include "key.hh"
#include "securearray.hh"

/**
 * @brief Namespace for cryptographic stuff
 *
 * Namespace for cryptographic stuff. Has no front-end and relies on
 * openssl.
 */
namespace yapet {
/**
 * @brief Converts the password into the 448bits key
 *
 * Converts the password into the key which is used by the other
 * cryptographic related classes.
 *
 * The key uses the maximum length of 448bits (56bytes) allowed
 * for blowfish.
 *
 * The key is computed using three passes. The first pass hashes
 * the password using the sha1 algorithm. This hash is then
 * re-hashed using md5 which is then appended to the key generated
 * by the previous pass (sha1 + md5). The last pass hashes the
 * result of the former two passes using RIPEMD-160 and appended
 * the result to the key (sha1 + md5 + ripemd160).
 *
 * The initialization vector is computed by hashing the key using
 * the md5 algorithm and taking only the first eight bytes.
 */
class Key448 : public Key {
   private:
    /**
     * @brief Holds the key
     *
     * This is the key used to encrypt and decrypt data.
     */
    SecureArray _key;
    /**
     * @brief Holds the initialization vector
     *
     * The initialization vector used for encryption and
     * decryption.
     */
    SecureArray _ivec;

   public:
    Key448();

    Key448(const Key448& k);
    Key448& operator=(const Key448& k);

    Key448(Key448&& k);
    Key448& operator=(Key448&& key);
    ~Key448(){};

    void password(const SecureArray& password);

    SecureArray key() const { return _key; }

    SecureArray::size_type keySize() const { return _key.size(); }

    SecureArray ivec() const { return _ivec; }

    SecureArray::size_type ivecSize() const { return _ivec.size(); }

    //! Compares two keys for equality
    bool operator==(const Key448& k) const;
    bool operator==(const Key& k) const;

    //! Compares two keys for inequality
    bool operator!=(const Key448& k) const;
    bool operator!=(const Key& k) const;
};

}  // namespace yapet

#endif  // _KEY448_HH
