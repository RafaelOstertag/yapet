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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>

#include "consts.h"
#include "intl.h"
#include "pwgen.h"
#include "pwgen.hh"

using namespace YAPET::PWGEN;

/**
 * If possible, replaces multiple characters from same pool with character from
 * unused pools.
 *
 * Use only on zero terminated passwords!
 */
void PWGen::sanitize_password() {
    if ((static_cast<size_t>(cp->numPoolsAllocated()) > password_len) ||
        (cp->numPoolsNotRead() == 0)) /* No can do */
        return;

    for (size_t pwit_outer = 0; pwit_outer < password_len; pwit_outer++) {
        assert(password[pwit_outer] != '\0');

        char c_outer = password[pwit_outer];
        SUBPOOLS c_outer_pool = cp->fromPool(c_outer);
        // Search for characters from same pool
        for (size_t pwit_inner = 0; pwit_inner < password_len; pwit_inner++) {
            if (cp->numPoolsNotRead() == 0) return;
            assert(password[pwit_inner] != '\0');
            char c_inner = password[pwit_inner];
            if (pwit_inner != pwit_outer &&
                c_outer_pool == cp->fromPool(c_inner)) {
                const_cast<char*>(password)[pwit_outer] =
                    getCharFromUnusedPools();
                break;
            }
        }
        if (cp->numPoolsNotRead() == 0) return;
    }
}

char PWGen::getCharFromUnusedPools() {
    char suggestion = 0;
    if (cp->numPoolsNotRead() > 0) {
        // Find out which one(s) are not read
        int not_read = cp->getAllocatedPools() & ~cp->getPoolsWithRead();

        // Iterate over the bits, until we find a pool from
        // which we will grab a character. And since this pool
        // was never used before, we don't even have to care
        // about duplicates...
        for (int pool_it = YAPET::PWGEN::LETTERS;
             pool_it <= YAPET::PWGEN::OTHER; pool_it = pool_it << 1) {
            if (not_read & pool_it) {
                size_t pool_start;
                // Since cp->getPoolPos the length of the pool
                // returns, we use this to get the random
                // number which we simply can add to
                // pool_start.
                //
                // We must not forget to subtract 1 from the
                // return value, else we might get an out of
                // range error
                size_t random_val =
                    (*rng)(cp->getPoolPos((YAPET::PWGEN::SUBPOOLS)pool_it,
                                          &pool_start) -
                           1);
                suggestion = (*cp)[pool_start + random_val];
                return suggestion;
            }
        }  // for (int i=...
    } else {
        assert(0);
        throw std::logic_error(
            _("Cannot get character from unused pool because no unused pools "
              "are available"));
    }
    assert(0);
    return -1;
}

void PWGen::init(int p, RNGENGINE rnge) {
    cp = new CharacterPool(p);
    rng = new RNG(rnge);
}

PWGen::PWGen(SUBPOOLS p, RNGENGINE rnge)
    : cp(0), rng(0), password(0), password_len(0) {
    init(p, rnge);
    assert(cp != 0);
    assert(rng != 0);
    assert(password == 0);
}

PWGen::PWGen(int p, RNGENGINE rnge)
    : cp(0), rng(0), password(0), password_len(0) {
    init(p, rnge);
    assert(cp != 0);
    assert(rng != 0);
    assert(password == 0);
}

//
// Copy Constructor
//
PWGen::PWGen(const PWGen& pw) : cp(0), rng(0), password(0), password_len(0) {
    assert(pw.cp != 0);
    assert(pw.rng != 0);
    cp = new CharacterPool(*(pw.cp));
    rng = new RNG(*(pw.rng));
    assert(cp != 0);
    assert(rng != 0);

    if (pw.password != 0) {
        assert(pw.password_len > 0);
        password = new char[pw.password_len + 1];
        std::memcpy((void*)password, pw.password, pw.password_len);
        // Don't forget to zero terminate!
        const_cast<char*>(password)[pw.password_len + 1] = '\0';
        password_len = pw.password_len;
    }
}

PWGen::~PWGen() {
    assert(cp != 0);
    assert(rng != 0);
    delete cp;
    delete rng;

    if (password_len != 0) assert(password != 0);

    if (password != 0) {
        assert(password_len > 0);
        std::memset((void*)password, 0, password_len);
        delete[] password;
    }
}

void PWGen::setNewPool(int p) {
    assert(cp != 0);
    delete cp;
    cp = new CharacterPool(p);
    assert(cp != 0);
}

void PWGen::setNewRNG(RNGENGINE rnge) {
    assert(rng != 0);
    delete rng;
    rng = new RNG(rnge);
    assert(rng != 0);
}

void PWGen::generatePassword(size_t len) {
    if (len == 0) return;

    if (password != 0) {
        assert(password_len > 0);
        std::memset((void*)password, 0, password_len);
        delete[] password;
    }

    password_len = len;
    password = new char[password_len + 1];
    cp->resetPoolsWithRead();

    for (size_t pw_it = 0; pw_it < password_len; pw_it++) {
    RESTART:
        char suggestion = (*cp)[(*rng)(cp->getPoolLength())];
        if (static_cast<size_t>(cp->getPoolLength()) >= password_len) {
            // We can avoid repeating characters
            for (size_t pos = 0; pos < pw_it; pos++) {
                if (suggestion == password[pos]) {
                    //
                    // We found a duplicate
                    //
                    if (static_cast<size_t>(cp->numPoolsAllocated()) >=
                            password_len &&
                        cp->numPoolsNotRead() > 0) {
                        // make sure all pools are read from
                        suggestion = getCharFromUnusedPools();
                        break;
                    }
                }
            }
        }

        // We want to avoid spaces at the beginning or end of the password
        if ((pw_it == 0) || (pw_it == password_len - 1)) {
#if defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
#ifdef HAVE_ISBLANK

            if (isblank(suggestion) != 0) goto RESTART;

#else

            if (std::isspace(suggestion) != 0) goto RESTART;

#endif  // HAVE_ISBLANK
#else   // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)

            if (suggestion == ' ') goto RESTART;

#endif     // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
        }  // if ( (i == 0) || (i == password_len - 1) )

        const_cast<char*>(password)[pw_it] = suggestion;
    }  // for (size_t i = 0; i < password_len; i++)

    // Don't forget to zero terminate!
    const_cast<char*>(password)[password_len] = '\0';

    sanitize_password();
#ifndef NDEBUG
    // Make sure we used at least one character from each pool
    if (static_cast<size_t>(cp->numPoolsAllocated()) <= password_len) {
        assert(cp->numPoolsNotRead() == 0);
    }
#endif
}

const char* PWGen::getPassword() const {
    assert(((password != 0) && (password_len > 0)) ||
           ((password == 0) && (password_len == 0)));

    if (password == 0) return 0;

    assert(password[password_len] == '\0');
    return password;
}

const PWGen& PWGen::operator=(const PWGen& pw) {
    assert(cp != 0);
    assert(rng != 0);

    if (&pw == this) return *this;

    delete cp;
    cp = new CharacterPool(*(pw.cp));
    delete rng;
    rng = new RNG(*(pw.rng));

    if (password != 0) {
        assert(password_len > 0);
        std::memset((void*)password, 0, password_len);
        delete[] password;
    }

    if (pw.password != 0) {
        assert(pw.password_len > 0);
        password = new char[pw.password_len];
        std::memcpy((void*)password, pw.password, pw.password_len);
        // Don't forget to zero terminate
        const_cast<char*>(password)[pw.password_len + 1] = '\0';
        password_len = pw.password_len;
    }

    return *this;
}

using namespace yapet::pwgen;

yapet::pwgen::CharacterPool& PasswordGenerator::getRandomCharacterPool() {
    auto randomByte = _rng->getNextByte();
    auto listIndex = randomByte % _concretePools.size();
    return _concretePools[listIndex];
}

std::uint8_t PasswordGenerator::getRandomCharacterFromPool(
    const yapet::pwgen::CharacterPool& pool) {
    auto randomByte = _rng->getNextByte();
    return pool[randomByte];
}

PasswordGenerator::PasswordGenerator(RNGENGINE rngEngine, POOLS pool)
    : PasswordGenerator{rngEngine, static_cast<int>(pool)} {}

PasswordGenerator::PasswordGenerator(RNGENGINE rngEngine, int pools)
    : _rng{getRng(rngEngine)},
      _rngEngine{rngEngine},
      _characterPools{pools},
      _concretePools{getPools(_characterPools)} {}

PasswordGenerator::PasswordGenerator(PasswordGenerator&& other)
    : _rng{std::move(other._rng)},
      _rngEngine{other._rngEngine},
      _characterPools{other._characterPools},
      _concretePools{std::move(other._concretePools)} {}

PasswordGenerator& PasswordGenerator::operator=(PasswordGenerator&& other) {
    if (this == &other) {
        return *this;
    }

    _rng = std::move(other._rng);
    _rngEngine = other._rngEngine;
    _characterPools = other._characterPools;
    _concretePools = std::move(other._concretePools);

    return *this;
}

void PasswordGenerator::characterPools(int pools) {
    _characterPools = pools;
    _concretePools = getPools(pools);
}

void PasswordGenerator::rngEngine(RNGENGINE rngEngine) {
    _rng = getRng(rngEngine);
    _rngEngine = rngEngine;
}

yapet::SecureArray PasswordGenerator::generatePassword(int size) {
    if (size < 1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("%d is not a valid password size"), size);
        throw std::invalid_argument(msg);
    }

    SecureArray password{size + 1};
    for (int i = 0; i < size; i++) {
        auto characterPool = getRandomCharacterPool();
        auto character = getRandomCharacterFromPool(characterPool);

        password[i] = character;
    }
    password[size] = '\0';
    return password;
}