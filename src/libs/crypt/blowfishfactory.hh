#ifndef _BLOWFISHFACTORY_HH
#define _BLOWFISHFACTORY_HH

#include "abstractcryptofactory.hh"
#include "blowfish.hh"
#include "key448.hh"
#include "yapet10file.hh"

namespace yapet {
class BlowfishFactory : public AbstractCryptoFactory {
   private:
    std::shared_ptr<Key> _key448;

   public:
    BlowfishFactory(const SecureArray& password,
                    const MetaData& keyingParameters);
    BlowfishFactory(const BlowfishFactory&) = delete;
    BlowfishFactory(BlowfishFactory&&) = delete;
    BlowfishFactory& operator=(const BlowfishFactory&) = delete;
    BlowfishFactory& operator=(BlowfishFactory&&) = delete;
    ~BlowfishFactory(){};

    virtual std::shared_ptr<AbstractCryptoFactory> newFactory(
        const SecureArray& password, const MetaData& keyingParameters) const;

    virtual std::unique_ptr<Crypto> crypto() const;

    virtual std::shared_ptr<Key> key() const { return _key448; }

    virtual std::unique_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const;
};
}  // namespace yapet

#endif