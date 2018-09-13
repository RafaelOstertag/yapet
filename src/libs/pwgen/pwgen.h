// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009-2010  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _PWGEN_H
#define _PWGEN_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "intl.h"

#include "charpool.h"
#include "rng.h"

namespace YAPET {
/**
 * @brief Namespace for pwgen.
 *
 * All password generator related classes belong to this namespace.
 */
namespace PWGEN {
/**
 * @brief Generates a password.
 *
 * Generates a more or less secure password depending on the character pool
 * used.
 *
 * @see CharacterPool
 */
class PWGen {
   private:
    CharacterPool* cp;
    RNG* rng;
    /**
     * @brief will hold the password.
     *
     * This will hold the (zero terminated) password. It is entierly managed by
     * this class. When PWGen returns a password, it is always a pointer to this
     * buffer.
     */
    const char* password;
    size_t password_len;

    void sanitize_password();
    char getCharFromUnusedPools();
    void init(int p, RNGENGINE rnge);

   public:
    PWGen(SUBPOOLS p, RNGENGINE rnge = AUTO);
    PWGen(int p, RNGENGINE rnge = AUTO);
    PWGen(const PWGen& pw);
    virtual ~PWGen();

    void setNewPool(int p);
    void setNewRNG(RNGENGINE rnge);
    void generatePassword(size_t len);
    const char* getPassword() const;
    inline RNGENGINE getRNGUsed() const { return rng->getRNGEngineUsed(); }
    inline int getCharacterPools() const {
        if (cp != 0) return cp->getAllocatedPools();
        return 0;
    }

    const PWGen& operator=(const PWGen& pw);
};
}  // namespace PWGEN
}  // namespace YAPET

#endif  // _PWGEN_H
