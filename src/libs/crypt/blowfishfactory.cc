#include "blowfishfactory.hh"

using namespace yapet;

BlowfishFactory::BlowfishFactory(const SecureArray& password)
    : _key448{new Key448{}} {
    _key448->password(password);
}

std::shared_ptr<AbstractCryptoFactory> BlowfishFactory::newFactory(
    const SecureArray& password) const {
    return std::shared_ptr<AbstractCryptoFactory>{
        new BlowfishFactory(password)};
}

std::unique_ptr<Crypto> BlowfishFactory::crypto() const {
    return std::unique_ptr<Crypto>{new Blowfish{_key448}};
}

std::unique_ptr<YapetFile> BlowfishFactory::file(const std::string& filename,
                                                 bool create,
                                                 bool secure) const {
    return std::unique_ptr<YapetFile>{
        new Yapet10File{filename, create, secure}};
}