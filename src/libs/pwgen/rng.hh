#ifndef _RNG_HH
#define _RNG_HH

#include <cstdint>
#include <limits>
#include <random>

namespace yapet {
namespace pwgen {

class Rng {
   private:
    class RngFunctor {
       private:
        Rng& _rng;

       public:
        using result_type = std::uint8_t;
        static std::uint8_t min() { return std::numeric_limits<std::uint8_t>::min(); }
        static std::uint8_t max() { return std::numeric_limits<std::uint8_t>::max(); }

        RngFunctor(Rng& rng) : _rng{rng} {}
        std::uint8_t operator()() { return _rng.readRandomInt(); }
    };
    int fd;
    std::uniform_int_distribution<std::uint8_t> intUniformDistribution;
    RngFunctor rngFunctor;

    std::uint8_t readRandomInt();

   public:
    Rng(std::uint8_t lo, std::uint8_t hi);
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