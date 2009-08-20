// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#ifndef _KEY_H
#define _KEY_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include <openssl/evp.h>

#include "yapetexception.h"

/**
 * @brief Namespace for cryptographic stuff
 *
 * Namespace for cryptographic stuff. Has no front-end and relies on
 * openssl.
 */
namespace YAPET {
    /**
     * @brief Converts the password into the key
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
    class Key {
        private:
            enum {
                /**
                 * The max length of the blowfish key in bytes (448 bits)
                 */
                KEYLENGTH = 56,
                /**
                 * The length of the output of md5 (128 bits)
                 */
                MD5_LEN = 16,

                /**
                 * The length of the output of sha1 (160 bits)
                 */
                SHA1_LEN = 20,
                /**
                 * The lenght of the output of ripemd-160 (160 bits)
                 */
                RIPEMD160_LEN = 20,
                /**
                 * The length of the initialization vector
                 */
                IVECLENGTH = 8
            };

            /**
             * @brief Holds the key
             *
             * This is the key used to encrypt and decrypt data.
             */
            uint8_t key[KEYLENGTH];
            /**
             * @brief Holds the initialization vector
             *
             * The initialization vector used for encryption and
             * decryption.
             */
            uint8_t IVec[IVECLENGTH];

            //! Cleanup routine
            void cleanup();

        public:
            //! Initializes the key
            Key (const char* password) throw (YAPETException);
            Key (const Key& k);
            ~Key();

            /**
             * @brief Gets the pointer to the key
             *
             * Returns the key and its length. The key is not
             * terminated by \c \\0. So make sure you read only \c
             * key_len bytes from the pointer returned.
             *
             * @param key_len reference to an integer receiving the
             * key length in bytes
             *
             * @return pointer to the array of unsigned 8bit integers
             * holding the key.
             */
            inline const uint8_t* getKey (int& key_len) const {
                key_len = KEYLENGTH;
                return key;
            }

            /**
             * @brief Gets the pointer to the key
             *
             * Returns the key only. Please note that the key is not
             * terminated by \c \\0, so make sure you read only as many
             * bytes as returned by \c size().
             *
             * @return pointer to the array of usigned 8bit integers
             * holding the key.
             */
            inline const uint8_t* getKey() const {
                return key;
            }

            /**
             * @brief Gets the pointer to the initialization vector
             *
             * Gets the initialization vector and its length. Please
             * remember that the initialization vector is not \c \\0
             * terminated. So make sure you read only \c ivec_len
             * bytes from the pointer returned.
             *
             * @param ivec_len a reference to an integer receiving the
             * length of the initialization vector.
             *
             * @return pointer to the array of unsigned 8bit integers
             * holding the initialization vector.
             */
            inline const uint8_t* getIVec (int& ivec_len) const {
                ivec_len = IVECLENGTH;
                return IVec;
            }

            /**
             * @brief Gets the initialization vector
             *
             * Gets the initialization vector. Please remember that
             * the initialization vector is not \c \\0 terminated. So
             * make sure you read only as many bytes as returned by \c
             * ivec_size().
             *
             * @return pointer to the array of unsigned 8bit integers
             * holding the initialization vector.
             */
            inline const uint8_t* getIVec() const {
                return IVec;
            }

            /**
             * @brief Returns the key length in bytes
             *
             * Returns the key length in bytes
             *
             * @return key length in bytes.
             */
            inline uint32_t size() const {
                return KEYLENGTH;
            }

            /**
             * @brief Returns the length of the initialization vector
             *
             * Returns the length of the initialization vector in
             * bytes.
             *
             * @return the size of the initialization vector in bytes.
             */
            inline uint32_t ivec_size() const {
                return IVECLENGTH;
            }

            /**
             * @brief Gets the pointer to the key
             *
             * Returns the key and its length. The key is not
             * terminated by \c \\0. So make sure you read only \c
             * key_len bytes from the pointer returned.
             *
             * @param key_len reference to an integer receiving the
             * key length in bytes
             *
             * @return pointer to the array of unsigned 8bit integers
             * holding the key.
             */
            inline const uint8_t* operator() (int& key_len) const {
                return getKey (key_len);
            }

            /**
             * @brief Gets the pointer to the key
             *
             * Returns the key only. Please note that the key is not
             * terminated by \c \\0, so make sure you read only as many
             * bytes as returned by \c size().
             *
             * @return pointer to the array of usigned 8bit integers
             * holding the key.
             */
            inline const uint8_t* operator() () const {
                return key;
            }

            /**
             * @brief Cast operator
             *
             * Returns the pointer to the key.
             */
            inline operator uint8_t*() {
                return key;
            }

            /**
             * @brief Cast operator
             *
             * Returns the pointer to the key.
             */
            inline operator const uint8_t*() const {
                return key;
            }

            const Key& operator= (const Key& k);
            //! Compares two keys for equality
            bool operator== (const Key& k) const;
            //! Compares two keys for inequality
            bool operator!= (const Key& k) const {
                return !operator== (k);
            }
    };

}

#endif // _KEY_H
