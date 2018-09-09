#include "aes256factory.hh"

using namespace yapet;

Aes256Factory::Aes256Factory(const SecureArray& password)
    : _key256{new Key256{}} {
    _key256->password(password);
}

std::shared_ptr<AbstractCryptoFactory> Aes256Factory::newFactory(
    const SecureArray& password) const {
    return std::shared_ptr<AbstractCryptoFactory>{new Aes256Factory(password)};
}

std::unique_ptr<Crypto> Aes256Factory::crypto() const {
    return std::unique_ptr<Crypto>{new Aes256{_key256}};
}

std::unique_ptr<YapetFile> Aes256Factory::file(const std::string& filename,
                                               bool create, bool secure) const {
    return std::unique_ptr<YapetFile>{
        new Yapet20File{filename, create, secure}};
}