#ifndef _ABSTRACTCRYPTOFACTORY_HH
#define _ABSTRACTCRYPTOFACTORY_HH

#include <memory>

#include "crypto.hh"
#include "yapetfile.hh"

namespace yapet {
/**
 * Abstracto Crypt Factory.
 *
 * Implementations are expected to provide means for specifying a password if
 * necessary.
 */
class AbstractCryptoFactory {
   public:
    virtual ~AbstractCryptoFactory() {}

    virtual std::shared_ptr<AbstractCryptoFactory> newFactory(
        const SecureArray& password) const = 0;
    virtual std::unique_ptr<Crypto> crypto() const = 0;
    virtual std::shared_ptr<Key> key() const = 0;
    virtual std::unique_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const = 0;
};
}  // namespace yapet

#endif