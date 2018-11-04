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

#ifndef _RNG_HH
#define _RNG_HH

#include <cstdint>
#include <random>
#include <rngengine.hh>

namespace yapet {
namespace pwgen {

class Rng {
   private:
    RngEngine _rngEngine;
    std::uniform_int_distribution<std::uint8_t> _distribution;

   public:
    Rng(std::uint8_t hi);
    Rng(const Rng& rng);
    Rng(Rng&& rng);
    Rng& operator=(const Rng& rng);
    Rng& operator=(Rng&& rng);

    std::uint8_t getNextInt();
    std::uint8_t operator()();
};

}  // namespace pwgen
}  // namespace yapet

#endif