#ifndef _CRYPTO_HH
#define _CRYPTO_HH

#include <openssl/evp.h>
#include <memory>

#include "key.hh"
#include "securearray.hh"

namespace yapet {
/**
 * Base class for encryption and decryption.
 */
class Crypto {
   private:
    std::shared_ptr<Key> _key;

   protected:
    virtual const EVP_CIPHER* getCipher() const = 0;

   public:
    Crypto(const std::shared_ptr<Key>& key);
    virtual ~Crypto(){};

    Crypto(const Crypto&);
    Crypto& operator=(const Crypto& c);

    Crypto(Crypto&& c);
    Crypto& operator=(Crypto&& c);

    virtual SecureArray encrypt(const SecureArray& plainText);
    virtual SecureArray decrypt(const SecureArray& cipherText);
};
}  // namespace yapet

#endif