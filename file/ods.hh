/**
 * \file
 *
 * Functions supporting the On-Disk-Structure of YAPET files.
 */
#ifndef _ENDIANESS_HH
#define _ENDIANESS_HH

#include <cstdint>
#include "config.h"

namespace yapet {
namespace {
template <typename T>
union Integer {
    T integer;
    std::uint8_t bytes[sizeof(T)];
};

template <typename T>
void reverseBytes(Integer<T>& integer) {
    Integer<T> reversed;
    auto integerSize = sizeof(integer.integer);
    for (auto i = 0lu; i < integerSize;
         reversed.bytes[(integerSize - 1) - i] = integer.bytes[i], i++)
        ;
    integer.integer = reversed.integer;
}
}  // namespace

/**
 * Convert an integer to On-Disk-Structure
 */
template <typename T>
T toODS(T odsInt) {
#ifdef WORDS_BIGENDIAN
    return odsInt;
#else
    Integer<T> ods{odsInt};
    reverseBytes(ods);
    return ods.integer;
#endif
}

/**
 * Convert an integer from On-Disk-Structure to host representation
 */
template <typename T>
T toHost(T hostInt) {
#ifdef WORDS_BIGENDIAN
    return hostInt;
#else
    Integer<T> ods{hostInt};
    reverseBytes(ods);
    return ods.integer;
#endif
}
}  // namespace yapet

#endif