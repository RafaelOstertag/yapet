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
    int _characterPools;

   public:
    PasswordGenerator(POOLS pool);
    PasswordGenerator(int pools);
    PasswordGenerator(const PasswordGenerator&) = delete;
    PasswordGenerator(PasswordGenerator&& other);

    PasswordGenerator& operator=(const PasswordGenerator&) = delete;
    PasswordGenerator& operator=(PasswordGenerator&& other);

    void characterPools(int pools);
    void characterPools(POOLS pool) { characterPools(static_cast<int>(pool)); }
    int characterPools() const { return _characterPools; }

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