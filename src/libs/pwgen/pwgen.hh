#ifndef _PWGEN_HH
#define _PWGEN_HH

#include <memory>

#include "characterpool.hh"
#include "rng.hh"
#include "securearray.hh"

namespace yapet {
namespace pwgen {
class PasswordGenerator {
   private:
    int _characterPools;

   public:
    PasswordGenerator(POOLS pool);
    PasswordGenerator(int pools);
    PasswordGenerator(const PasswordGenerator&) = delete;
    PasswordGenerator(PasswordGenerator&& other);

    PasswordGenerator& operator=(const PasswordGenerator&) = delete;
    PasswordGenerator& operator=(PasswordGenerator&& other);

    void characterPools(int pools);
    void characterPools(POOLS pool) { characterPools(static_cast<int>(pool)); }
    int characterPools() const { return _characterPools; }

    /**
     * Return a zero terminated random password.
     * 
     * \c size must be at least 1.
     */
    SecureArray generatePassword(int size);
};
}  // namespace pwgen
}  // namespace yapet

#endif