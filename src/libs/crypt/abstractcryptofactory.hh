#ifndef _ABSTRACTCRYPTOFACTORY_HH
#define _ABSTRACTCRYPTOFACTORY_HH

#include <memory>

#include "crypto.hh"
#include "yapetfile.hh"

namespace yapet {
class AbstractCryptoFactory {
   public:
    virtual ~AbstractCryptoFactory() {}
    //TODO: can we return unique_ptr?
    virtual std::shared_ptr<Crypto> crypto(
        const SecureArray& password) const = 0;
    // TODO: can we return unique_ptr?
    virtual std::shared_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const = 0;
};
}  // namespace yapet

#endif