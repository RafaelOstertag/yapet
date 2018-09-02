#ifndef _ABSTRACTCRYPTOFACTORY_HH
#define _ABSTRACTCRYPTOFACTORY_HH

#include <memory>

#include "crypto.hh"
#include "yapetfile.hh"

namespace yapet {
class AbstractCryptoFactory {
   public:
    virtual ~AbstractCryptoFactory() {}
    virtual std::shared_ptr<Crypto> crypto(
        const SecureArray& password) const = 0;
    virtual std::shared_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const = 0;
};
}  // namespace yapet

#endif