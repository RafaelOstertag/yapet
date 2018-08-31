#ifndef _CRYPTO_HH
#define _CRYPTO_HH

#include "key.h"
#include "securearray.hh"

namespace yapet {
class Crypto {
   public:
    virtual ~Crypto(){};

    virtual SecureArray encrypt(const SecureArray& plainText,
                                const YAPET::Key& key) = 0;
    virtual SecureArray decrypt(const SecureArray& cipherText,
                                const YAPET::Key& key) = 0;
};
}  // namespace yapet

#endif