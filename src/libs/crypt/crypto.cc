#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_EVP_CIPHER_CTX_INIT
#include <cstdlib>
#endif

#include "crypto.hh"
#include "intl.h"
#include "yapetexception.h"

using namespace yapet;

void Crypto::checkIVSizeOrThrow() {
    auto supportedIVSize{cipherIvecSize()};
    auto expectedIVSize{_key->ivecSize()};

    if (supportedIVSize != expectedIVSize) {
        std::string message{_("Expect cipher to support IV size ")};
        message += std::to_string(expectedIVSize);
        message += _(" but cipher supports only IV size ");
        message += std::to_string(supportedIVSize);
        throw YAPET::YAPETException(message);
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
    if (getCipher() == nullptr)
        throw YAPET::YAPETException{_("Unable to get cipher")};

    checkIVSizeOrThrow();
}

EVP_CIPHER_CTX* Crypto::initializeOrThrow(MODE mode) {
    EVP_CIPHER_CTX* context = createContext();

    auto success = EVP_CipherInit_ex(context, getCipher(), nullptr,
                                     *_key->key(), *_key->ivec(), mode);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw YAPET::YAPETException(_("Error initializing cipher"));
    }

    success = EVP_CIPHER_CTX_set_key_length(context, _key->keySize());
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        std::string message{_("Cannot set key length on context to ")};
        message += std::to_string(_key->keySize());
        throw YAPET::YAPETException(message);
    }

    return context;
}

int Crypto::cipherBlockSize() { return EVP_CIPHER_block_size(getCipher()); }

int Crypto::cipherIvecSize() { return EVP_CIPHER_iv_length(getCipher()); }

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
        throw YAPET::YAPETException(_("Cannot encrypt empty plain text"));
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
        throw YAPET::YAPETEncryptionException(_("Error encrypting data"));
    }

    auto effectiveEncryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryEncryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw YAPET::YAPETEncryptionException(_("Error finalizing encryption"));
    }

    effectiveEncryptedDataLength += writtenDataLength;
    SecureArray encryptedData{effectiveEncryptedDataLength};

    destroyContext(context);

    return (encryptedData << temporaryEncryptedData);
}

SecureArray Crypto::decrypt(const SecureArray& cipherText) {
    if (cipherText.size() == 0) {
        throw YAPET::YAPETException(_("Cannot decrypt empty cipher text"));
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
        throw YAPET::YAPETEncryptionException(_("Error decrypting data"));
    }

    auto effectiveDecryptedDataLength = writtenDataLength;
    success = EVP_CipherFinal_ex(context,
                                 (*temporaryDecryptedData) + writtenDataLength,
                                 &writtenDataLength);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw YAPET::YAPETEncryptionException(
            _("Error finalizing decrypting data"));
    }

    effectiveDecryptedDataLength += writtenDataLength;
    SecureArray encryptedData{effectiveDecryptedDataLength};

    destroyContext(context);

    return (encryptedData << temporaryDecryptedData);
}