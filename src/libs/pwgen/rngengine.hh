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

#ifndef _RNGENGINE_HH
#define _RNGENGINE_HH

#include <array>
#include <cstdint>
#include <limits>

namespace yapet {
namespace pwgen {
class RngEngine {
   public:
    using result_type = std::uint8_t;
    static constexpr std::uint8_t max() {
        return std::numeric_limits<std::uint8_t>::max();
    }
    static constexpr std::uint8_t min() {
        return std::numeric_limits<std::uint8_t>::min();
    }

    RngEngine();
    ~RngEngine();
    RngEngine(const RngEngine& other);
    RngEngine(RngEngine&& other);
    RngEngine& operator=(const RngEngine& other);
    RngEngine& operator=(RngEngine&& other);

    std::uint8_t operator()();

   private:
    static constexpr int BYTE_CACHE_SIZE{8};
    static constexpr int EMPTY_CACHE{-1};
    int fd;
    std::array<std::uint8_t, BYTE_CACHE_SIZE> byteCache;
    int positionInCache;

    void fillCache();
    std::uint8_t readRandomByte();
};
}  // namespace pwgen
}  // namespace yapet
#endif