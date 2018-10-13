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

#ifndef _CRYPTO_HH
#define _CRYPTO_HH

#include <openssl/evp.h>
#include <memory>

#include "key.hh"
#include "securearray.hh"

namespace yapet {
/**
 * Base class for encryption and decryption.
 */
class Crypto {
   private:
    std::shared_ptr<Key> _key;

   protected:
    static constexpr auto SSL_SUCCESS{1};
    enum MODE { DECRYPTION = 0, ENCRYPTION = 1 };

    EVP_CIPHER_CTX* createContext();
    void destroyContext(EVP_CIPHER_CTX* context);
    EVP_CIPHER_CTX* initializeOrThrow(MODE mode);

    void checkIVSizeOrThrow();
    void validateCipherOrThrow();

    int cipherIvecSize() const;
    int cipherBlockSize() const;

    virtual const EVP_CIPHER* getCipher() const = 0;

   public:
    /**
     * Initializes the class with the given key, which is used for
     * encryption and decryption.
     *
     * The constructor tries to set the key length of the cipher used to
     * the length of the key provided. If this fails, a \c YAPETException is
     * thrown.
     *
     * @param k the key used for encryption/decryption.
     */
    Crypto(const std::shared_ptr<Key>& key);
    virtual ~Crypto(){};

    Crypto(const Crypto&);
    Crypto& operator=(const Crypto& c);

    Crypto(Crypto&& c);
    Crypto& operator=(Crypto&& c);

    /**
     * Encrypt data using the cipher provided by \c getCipher().
     *
     * @throw YAPETException in case the key length of the cipher cannot be
     * set to the length of the key provided.
     *
     * @throw YAPETEncryptionException in case of cipher errors.
     */
    virtual SecureArray encrypt(const SecureArray& plainText);
    /**
     * Decrypt data using the cipher provided by \c getCipher().
     *
     * @throw YAPETException in case the key length of the cipher cannot be
     * set to the length of the key provided.
     *
     * @throw YAPETEncryptionException in case of cipher errors.
     */
    virtual SecureArray decrypt(const SecureArray& cipherText);

    std::shared_ptr<Key> getKey() const { return _key; }
};
}  // namespace yapet

#endif