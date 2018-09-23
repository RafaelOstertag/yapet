#ifndef _AES256FACTORY_HH
#define _AES256FACTORY_HH

#include "abstractcryptofactory.hh"
#include "aes256.hh"
#include "key256.hh"
#include "yapet20file.hh"

namespace yapet {
class Aes256Factory : public AbstractCryptoFactory {
   private:
    std::shared_ptr<Key> _key256;

   public:
    Aes256Factory(const SecureArray& password,
                  const MetaData& keyingParameters);
    Aes256Factory(const Aes256Factory&) = delete;
    Aes256Factory(Aes256Factory&&) = delete;
    Aes256Factory& operator=(const Aes256Factory&) = delete;
    Aes256Factory& operator=(Aes256Factory&&) = delete;
    ~Aes256Factory(){};

    virtual std::shared_ptr<AbstractCryptoFactory> newFactory(
        const SecureArray& password, const MetaData& keyingParameters) const;

    virtual std::unique_ptr<Crypto> crypto() const;

    virtual std::shared_ptr<Key> key() const { return _key256; }

    virtual std::unique_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const;
};
}  // namespace yapet

#endif