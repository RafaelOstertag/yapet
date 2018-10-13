/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

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
using record_size_type = std::uint32_t;
namespace {
template <class T>
union Integer {
    T integer;
    std::uint8_t bytes[sizeof(T)];
};

template <class T>
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
template <class T>
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
template <class T>
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