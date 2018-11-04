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

#include <openssl/rand.h>
#include <cstdio>
#include <cstring>

#include "aes256.hh"
#include "consts.h"
#include "cryptoerror.hh"
#include "intl.h"
#include "logger.hh"

using namespace yapet;

SecureArray Aes256::randomIV() const {
    SecureArray ivec{cipherIvecSize()};

    auto result = RAND_bytes((*ivec), ivec.size());
    if (result != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} +
                    ": Cannot generate random initialization vector");
        throw CipherError{_("Cannot generate random initialization vector")};
    }

    return ivec;
}

SecureArray Aes256::extractIVFromRecord(const SecureArray& record) const {
    if (record.size() < cipherIvecSize()) {
        LOG_MESSAGE(std::string{__func__} +
                    ": Record does not contain initialization vector");
        throw CipherError{_("Record does not contain initialization vector")};
    }

    SecureArray ivec{cipherIvecSize()};
    ivec << record;
    return ivec;
}

SecureArray Aes256::extractCipherTextFromRecord(
    const SecureArray& record) const {
    auto ivecSize{cipherIvecSize()};
    if (record.size() <= ivecSize) {
        LOG_MESSAGE(std::string{__func__} +
                    ": Record does not contain encrypted data");
        throw CipherError{_("Record does not contain encrypted data")};
    }

    auto pointerToCipherText{*record + ivecSize};

    auto cipherTextSize{record.size() - ivecSize};

    return toSecureArray(pointerToCipherText, cipherTextSize);
}

void Aes256::checkIVSizeOrThrow(const SecureArray& ivec) {
    auto supportedIVSize{cipherIvecSize()};
    auto expectedIVSize{ivec.size()};

    if (supportedIVSize != expectedIVSize) {
        LOG_MESSAGE(std::string{__func__} + ": IV size mismatch");
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expect cipher to support IV size %d but cipher "
                        "supports only IV size %d"),
                      expectedIVSize, supportedIVSize);

        throw CipherError{msg};
    }
}

void Aes256::validateCipherOrThrow(const SecureArray& ivec) {
    if (getCipher() == nullptr) throw CipherError{_("Unable to get cipher")};

    checkIVSizeOrThrow(ivec);
}

EVP_CIPHER_CTX* Aes256::initializeOrThrow(const SecureArray& ivec, MODE mode) {
    EVP_CIPHER_CTX* context = createContext();

    auto success = EVP_CipherInit_ex(context, getCipher(), nullptr,
                                     *getKey()->key(), *ivec, mode);
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": Error initializing cipher");
        destroyContext(context);
        throw CipherError{_("Error initializing cipher")};
    }

    success = EVP_CIPHER_CTX_set_key_length(context, getKey()->keySize());
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": Error setting key length");
        destroyContext(context);
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot set key length on context to %d"),
                      getKey()->keySize());
        throw CipherError{msg};
    }

    return context;
}

Aes256::Aes256(const std::shared_ptr<Key>& key) : Crypto(key) {}

Aes256::Aes256(const Aes256& c) : Crypto(c) {}

Aes256& Aes256::operator=(const Aes256& c) {
    if (this == &c) return *this;

    Crypto::operator=(c);

    return *this;
}

Aes256::Aes256(Aes256&& c) : Crypto{c} {}

Aes256& Aes256::operator=(Aes256&& c) {
    if (this == &c) return *this;

    Crypto::operator=(c);

    return *this;
}

SecureArray Aes256::encrypt(const SecureArray& plainText) {
    if (plainText.size() == 0) {
        LOG_MESSAGE(std::string{__func__} +
                    ": Cannot encrypt empty plain text");
        throw EncryptionError{_("Cannot encrypt empty plain text")};
    }

    SecureArray ivec{randomIV()};

    validateCipherOrThrow(ivec);
    EVP_CIPHER_CTX* context = initializeOrThrow(ivec, ENCRYPTION);

    auto blockSize = cipherBlockSize();

    SecureArray temporaryEncryptedData{plainText.size() + (2 * blockSize)};
    yapet::SecureArray::size_type writtenDataLength;

    auto success =
        EVP_CipherUpdate(context, *temporaryEncryptedData, &writtenDataLength,
                         *plainText, plainText.size());
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": EVP_CipherUpdate failure");
        destroyContext(context);
        throw EncryptionError{_("Error encrypting data")};
    }

    auto effectiveEncryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryEncryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": EVP_CipherFinal_ex failure");
        destroyContext(context);
        throw EncryptionError{_("Error finalizing encryption")};
    }

    effectiveEncryptedDataLength += writtenDataLength;

    LOG_MESSAGE(std::string{__func__} + ": " +
                std::to_string(effectiveEncryptedDataLength) +
                "bytes encrypted");
    assert(effectiveEncryptedDataLength <= temporaryEncryptedData.size());

    SecureArray encryptedData{effectiveEncryptedDataLength};

    destroyContext(context);

    return ivec + (encryptedData << temporaryEncryptedData);
}

SecureArray Aes256::decrypt(const SecureArray& cipherText) {
    if (cipherText.size() == 0) {
        LOG_MESSAGE(std::string{__func__} +
                    ": Cannot decrypt empty cipher text");
        throw EncryptionError{_("Cannot decrypt empty cipher text")};
    }

    SecureArray ivec{extractIVFromRecord(cipherText)};

    validateCipherOrThrow(ivec);

    EVP_CIPHER_CTX* context = initializeOrThrow(ivec, DECRYPTION);

    auto blockSize = cipherBlockSize();

    SecureArray cipherTextOnly{extractCipherTextFromRecord(cipherText)};

    SecureArray temporaryDecryptedData{cipherTextOnly.size() + blockSize};
    int writtenDataLength;

    auto success =
        EVP_CipherUpdate(context, *temporaryDecryptedData, &writtenDataLength,
                         *cipherTextOnly, cipherTextOnly.size());
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": EVP_CipherUpdate failure");
        destroyContext(context);
        throw EncryptionError{_("Error decrypting data")};
    }

    auto effectiveDecryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryDecryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        LOG_MESSAGE(std::string{__func__} + ": EVP_CipherFinal_ex failure");
        destroyContext(context);
        throw EncryptionError{_("Error finalizing decrypting data")};
    }

    effectiveDecryptedDataLength += writtenDataLength;

    LOG_MESSAGE(std::string{__func__} + ": " +
                std::to_string(effectiveDecryptedDataLength) +
                "bytes decrypted");

    assert(effectiveDecryptedDataLength <= temporaryDecryptedData.size());

    SecureArray decryptedData{effectiveDecryptedDataLength};

    destroyContext(context);

    return (decryptedData << temporaryDecryptedData);
}