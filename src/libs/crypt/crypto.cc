#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "crypto.hh"
#include "intl.h"
#include "yapetexception.h"

namespace {
constexpr auto SSL_SUCCESS{1};
constexpr auto SSL_FAILURE{0};

enum MODE { DECRYPTION = 0, ENCRYPTION = 1 };

inline void checkKeySizeOrThrow(int expectedKeySize, int supportedKeySize) {
    if (supportedKeySize < expectedKeySize) {
        std::string message = _("Expect cipher to support key size ");
        message += expectedKeySize;
        message += _(" but cipher supports only key size ");
        message += supportedKeySize;
        throw YAPET::YAPETException(message);
    }
}

inline void checkIVSizeOrThrow(int expectedIVSize, int supportedIVSize) {
    if (supportedIVSize != expectedIVSize) {
        std::string message = _("Expect cipher to support IV size ");
        message += expectedIVSize;
        message += _(" but cipher supports only IV size ");
        message += supportedIVSize;
        throw YAPET::YAPETException(message);
    }
}

inline EVP_CIPHER_CTX* createContext() {
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

inline void destroyContext(EVP_CIPHER_CTX* context) {
#ifdef HAVE_EVP_CIPHER_CTX_CLEANUP
    EVP_CIPHER_CTX_cleanup(context);
    std::free(context);
#elif HAVE_EVP_CIPHER_CTX_FREE
    EVP_CIPHER_CTX_free(context);
#else
#error "Neither EVP_CIPHER_CTX_cleanup() nor EVP_CIPHER_CTX_free() available"
#endif
}

inline void validateCipherOrThrow(const EVP_CIPHER* cipher, int keySize,
                                  int ivSize) {
    if (cipher == 0) throw YAPET::YAPETException(_("Unable to get cipher"));

    checkKeySizeOrThrow(keySize, EVP_CIPHER_key_length(cipher));
    checkIVSizeOrThrow(ivSize, EVP_CIPHER_iv_length(cipher));
}

inline EVP_CIPHER_CTX* initializetOrThrow(
    const EVP_CIPHER* cipher, const std::shared_ptr<yapet::Key>& key,
    MODE mode) {
    EVP_CIPHER_CTX* context = createContext();

    auto success = EVP_CipherInit_ex(context, cipher, nullptr, *key->key(),
                                     *key->ivec(), mode);
    if (success != SSL_SUCCESS) {
        destroyContext(context);
        throw YAPET::YAPETException(_("Error initializing cipher"));
    }

    EVP_CIPHER_CTX_set_key_length(context, key->keySize());

    return context;
}

inline int cipherBlockSize(const EVP_CIPHER* cipher) {
    return EVP_CIPHER_block_size(cipher);
}

}  // namespace

using namespace yapet;

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
    validateCipherOrThrow(getCipher(), _key->keySize(), _key->ivecSize());
    EVP_CIPHER_CTX* context = initializetOrThrow(getCipher(), _key, ENCRYPTION);

    auto blockSize = cipherBlockSize(getCipher());

    SecureArray temporaryEncryptedData{plainText.size() + (2 * blockSize)};
    int writtenDataLength;

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

    return (encryptedData << temporaryEncryptedData);
}

SecureArray Crypto::decrypt(const SecureArray& cipherText) {
    validateCipherOrThrow(getCipher(), _key->keySize(), _key->ivecSize());
    EVP_CIPHER_CTX* context = initializetOrThrow(getCipher(), _key, DECRYPTION);

    auto blockSize = cipherBlockSize(getCipher());

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
        throw YAPET::YAPETEncryptionException(_("Error finalizing decryption"));
    }

    effectiveDecryptedDataLength += writtenDataLength;
    SecureArray encryptedData{effectiveDecryptedDataLength};

    return (encryptedData << temporaryDecryptedData);
}