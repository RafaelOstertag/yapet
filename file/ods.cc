#include <cstdint>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ods.hh>

using namespace yapet;

union UnsignedIntegerWord {
    std::uint32_t integer;
    std::uint16_t halfWord1;
    std::uint16_t halfWord2;
};

inline void swapHalfWords(UnsignedIntegerWord& unsignedIntegerWord) {
    std::uint16_t tmpHalfWord = unsignedIntegerWord.halfWord2;
    unsignedIntegerWord.halfWord2 = unsignedIntegerWord.halfWord1;
    unsignedIntegerWord.halfWord1 = tmpHalfWord;
}

std::uint32_t toODS(std::uint32_t odsInt) {
#ifdef WORDS_BIGENDIAN
    return odsInt;
#else
    UnsignedIntegerWord ods{odsInt};
    swapHalfWords(ods);
    return ods.integer;
#endif
}

std::uint32_t toHost(std::uint32_t hostInt) {
#ifdef WORDS_BIGENDIAN
    return hostInt;
#else
    UnsignedIntegerWord ods{hostInt};
    swapHalfWords(ods);
    return ods.integer;
#endif
}
