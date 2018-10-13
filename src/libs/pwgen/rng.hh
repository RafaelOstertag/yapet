#ifndef _RNG_HH
#define _RNG_HH

#include <array>
#include <cstdint>
namespace yapet {
namespace pwgen {

class Rng {
   private:
    static constexpr int BYTE_CACHE_SIZE{8};
    static constexpr int EMPTY_CACHE{-1};
    int fd;
    std::array<std::uint8_t, BYTE_CACHE_SIZE> byteCache;
    int positionInCache;
    std::uint8_t max;

    void fillCache();
    std::uint8_t readRandomByte();

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