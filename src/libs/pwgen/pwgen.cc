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
#include "pwgen.hh"

using namespace yapet::pwgen;

PasswordGenerator::PasswordGenerator(RNGENGINE rngEngine, POOLS pool)
    : PasswordGenerator{rngEngine, static_cast<int>(pool)} {}

PasswordGenerator::PasswordGenerator(RNGENGINE rngEngine, int pools)
    : _rng{getRng(rngEngine)}, _rngEngine{rngEngine}, _characterPools{pools} {}

PasswordGenerator::PasswordGenerator(PasswordGenerator&& other)
    : _rng{std::move(other._rng)},
      _rngEngine{other._rngEngine},
      _characterPools{other._characterPools} {}

PasswordGenerator& PasswordGenerator::operator=(PasswordGenerator&& other) {
    if (this == &other) {
        return *this;
    }

    _rng = std::move(other._rng);
    _rngEngine = other._rngEngine;
    _characterPools = other._characterPools;

    return *this;
}

void PasswordGenerator::characterPools(int pools) { _characterPools = pools; }

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

    CharacterPool pools{getPools(_characterPools)};

    SecureArray password{size + 1};
    for (int i = 0; i < size; i++) {
        password[i] = pools[_rng->getNextByte()];
    }
    password[size] = '\0';
    return password;
}