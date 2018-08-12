/**
 * \file
 *
 * Functions supporting the On-Disk-Structure of YAPET files.
 */
#ifndef _ENDIANESS_HH
#define _ENDIANESS_HH

#include <cstdint>

namespace yapet {

/**
 * Convert integer to On-Disk-Structure
 */
std::uint32_t toODS(std::uint32_t);

/**
 * Convert integer from On-Disk-Structure to host representation
 */
std::uint32_t toHost(std::uint32_t);

}  // namespace yapet

#endif