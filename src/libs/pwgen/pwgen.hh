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
    std::unique_ptr<RngInterface> _rng;
    RNGENGINE _rngEngine;
    int _characterPools;
    std::vector<CharacterPool> _concretePools;

    CharacterPool& getRandomCharacterPool();
    std::uint8_t getRandomCharacterFromPool(const CharacterPool& pool);

   public:
    PasswordGenerator(RNGENGINE rngEngine, POOLS pool);
    PasswordGenerator(RNGENGINE rngEngine, int pools);
    PasswordGenerator(const PasswordGenerator&) = delete;
    PasswordGenerator(PasswordGenerator&& other);

    PasswordGenerator& operator=(const PasswordGenerator&) = delete;
    PasswordGenerator& operator=(PasswordGenerator&& other);

    void characterPools(int pools);
    void characterPools(POOLS pool) { characterPools(static_cast<int>(pool)); }
    int characterPools() const { return _characterPools; }

    RNGENGINE rngEngine() const { return _rngEngine; }
    void rngEngine(RNGENGINE rngEngine);

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