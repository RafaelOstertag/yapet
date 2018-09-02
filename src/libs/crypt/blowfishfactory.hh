#ifndef _BLOWFISHFACTORY_HH
#define _BLOWFISHFACTORY_HH

#include "abstractcryptofactory.hh"
#include "blowfish.hh"
#include "key448.hh"
#include "yapet10file.hh"

namespace yapet {
class BlowfishFactory : public AbstractCryptoFactory {
   public:
    ~BlowfishFactory(){};

    virtual std::shared_ptr<Crypto> crypto(const SecureArray& password) const {
        std::shared_ptr<Key> key{new Key448{}};
        key->password(password);

        return std::shared_ptr<Crypto>{new Blowfish{key}};
    }

    virtual std::shared_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const {
        return std::shared_ptr<YapetFile>{
            new Yapet10File{filename, create, secure}};
    }
};
}  // namespace yapet

#endif