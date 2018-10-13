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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_EVP_CIPHER_CTX_INIT
#include <cstdlib>
#endif

#include <cstdio>

#include "consts.h"
#include "crypto.hh"
#include "cryptoerror.hh"
#include "intl.h"

using namespace yapet;

void Crypto::checkIVSizeOrThrow() {
    auto supportedIVSize{cipherIvecSize()};
    auto expectedIVSize{_key->ivecSize()};

    if (supportedIVSize != expectedIVSize) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expect cipher to support IV size %d but cipher "
                        "supports only IV size %d"),
                      expectedIVSize, supportedIVSize);
        throw CipherError{msg};
    }
}

EVP_CIPHER_CTX* Crypto::createContext() {
#ifdef HAVE_EVP_CIPHER_CTX_INIT
    EVP_CIPHER_CTX* context =
        (EVP_CIPHER_CTX*)std::malloc(sizeof(EVP_CIPHER_CTX));
    EVP_CIPHER_CTX_init(context);
    return context;
#elif HAVE_EVP_CIPHER_CTX_NEW
    return EVP_CIPHER_CTX_new();
#else
#error "Neither EVP_CIPHER_CTX_init() nor EVP_CIPHER_CTX_new() available"
#endif
}

void Crypto::destroyContext(EVP_CIPHER_CTX* context) {
#ifdef HAVE_EVP_CIPHER_CTX_CLEANUP
    EVP_CIPHER_CTX_cleanup(context);
    std::free(context);
#elif HAVE_EVP_CIPHER_CTX_FREE
    EVP_CIPHER_CTX_free(context);
#else
#error "Neither EVP_CIPHER_CTX_cleanup() nor EVP_CIPHER_CTX_free() available"
#endif
}

void Crypto::validateCipherOrThrow() {
    if (getCipher() == nullptr) throw CipherError{_("Unable to get cipher")};

    checkIVSizeOrThrow();
}

EVP_CIPHER_CTX* Crypto::initializeOrThrow(MODE mode) {
    EVP_CIPHER_CTX* context = createContext();

    auto success = EVP_CipherInit_ex(context, getCipher(), nullptr,
                                     *_key->key(), *_key->ivec(), mode);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw CipherError{_("Error initializing cipher")};
    }

    success = EVP_CIPHER_CTX_set_key_length(context, _key->keySize());
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot set key length on context to %d"),
                      _key->keySize());
        throw CipherError{msg};
    }

    return context;
}

int Crypto::cipherBlockSize() const {
    return EVP_CIPHER_block_size(getCipher());
}

int Crypto::cipherIvecSize() const { return EVP_CIPHER_iv_length(getCipher()); }

Crypto::Crypto(const std::shared_ptr<yapet::Key>& key) : _key{key} {}

Crypto::Crypto(const Crypto& c) : _key{c._key} {}

Crypto& Crypto::operator=(const Crypto& c) {
    if (this == &c) return *this;

    _key = c._key;
    return *this;
}

Crypto::Crypto(Crypto&& c) : _key{std::move(c._key)} {}

Crypto& Crypto::operator=(Crypto&& c) {
    if (this == &c) return *this;

    _key = std::move(c._key);
    return *this;
}

SecureArray Crypto::encrypt(const SecureArray& plainText) {
    if (plainText.size() == 0) {
        throw EncryptionError{_("Cannot encrypt empty plain text")};
    }

    validateCipherOrThrow();
    EVP_CIPHER_CTX* context = initializeOrThrow(ENCRYPTION);

    auto blockSize = cipherBlockSize();

    SecureArray temporaryEncryptedData{plainText.size() + (2 * blockSize)};
    yapet::SecureArray::size_type writtenDataLength;

    auto success =
        EVP_CipherUpdate(context, *temporaryEncryptedData, &writtenDataLength,
                         *plainText, plainText.size());
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw EncryptionError{_("Error encrypting data")};
    }

    auto effectiveEncryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryEncryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw EncryptionError{_("Error finalizing encryption")};
    }

    effectiveEncryptedDataLength += writtenDataLength;
    assert(effectiveEncryptedDataLength <= temporaryEncryptedData.size());

    SecureArray encryptedData{effectiveEncryptedDataLength};

    destroyContext(context);

    return (encryptedData << temporaryEncryptedData);
}

SecureArray Crypto::decrypt(const SecureArray& cipherText) {
    if (cipherText.size() == 0) {
        throw EncryptionError{_("Cannot decrypt empty cipher text")};
    }

    validateCipherOrThrow();

    EVP_CIPHER_CTX* context = initializeOrThrow(DECRYPTION);

    auto blockSize = cipherBlockSize();

    SecureArray temporaryDecryptedData{cipherText.size() + blockSize};
    int writtenDataLength;

    auto success =
        EVP_CipherUpdate(context, *temporaryDecryptedData, &writtenDataLength,
                         *cipherText, cipherText.size());
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw EncryptionError{_("Error decrypting data")};
    }

    auto effectiveDecryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryDecryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw EncryptionError{_("Error finalizing decrypting data")};
    }

    effectiveDecryptedDataLength += writtenDataLength;
    assert(effectiveDecryptedDataLength <= temporaryDecryptedData.size());

    SecureArray decryptedData{effectiveDecryptedDataLength};

    destroyContext(context);

    return (decryptedData << temporaryDecryptedData);
}