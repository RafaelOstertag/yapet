#ifndef _RNG_HH
#define _RNG_HH

#include <cstdint>
#include <limits>
#include <random>

namespace yapet {
namespace pwgen {

class Rng {
   private:
    int fd;
    std::uint8_t max;

   public:
    Rng(std::uint8_t hi);
    ~Rng();
    Rng(const Rng& rng);
    Rng(Rng&& rng);
    Rng& operator=(const Rng& rng);
    Rng& operator=(Rng&& rng);

    std::uint8_t getNextInt();
};

}  // namespace pwgen
}  // namespace yapet

#endif