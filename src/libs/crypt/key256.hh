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

#ifndef _KEY256_HH
#define _KEY256_HH 1

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
 * @brief Converts the password into the 256bits key
 *
 * Converts the password into the key which is used by the other
 * cryptographic related classes.
 *
 * The key uses the maximum length of 256bits (32bytes) allowed
 * for AES 256.
 *
 * The key is computed hashing the password using SHA-256
 *
 * This class does not support initialization vector by deriving it from the
 * password. When using this class, the initialization vector must be obtained
 * by other means.
 */
class Key256 : public Key {
   private:
    /**
     * @brief Holds the key
     *
     * This is the key used to encrypt and decrypt data.
     */
    SecureArray _key;

    MetaData _keyingParameters;

   public:
    static MetaData newDefaultKeyingParameters();

    Key256();

    Key256(const Key256& k);
    Key256& operator=(const Key256& k);

    Key256(Key256&& k);
    Key256& operator=(Key256&& key);
    ~Key256(){};

    virtual void keyingParameters(const MetaData& parameters);

    virtual const MetaData& keyingParameters() const;

    void password(const SecureArray& password);

    SecureArray key() const { return _key; }

    SecureArray::size_type keySize() const { return _key.size(); }

    SecureArray ivec() const { return SecureArray{}; }

    SecureArray::size_type ivecSize() const { return 0; }

    //! Compares two keys for equality
    bool operator==(const Key256& k) const;
    bool operator==(const Key& k) const;

    //! Compares two keys for inequality
    bool operator!=(const Key256& k) const;
    bool operator!=(const Key& k) const;
};

}  // namespace yapet

#endif  // _KEY256_HH
