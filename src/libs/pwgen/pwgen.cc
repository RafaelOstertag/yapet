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
#include <limits>

#include "consts.h"
#include "intl.h"
#include "pwgen.hh"

using namespace yapet::pwgen;

namespace {
int countPools(int pools) {
    int numberOfPools = 0;
    if (isLetters(pools)) {
        numberOfPools++;
    }
    if (isDigits(pools)) {
        numberOfPools++;
    }
    if (isPunct(pools)) {
        numberOfPools++;
    }
    if (isSpecial(pools)) {
        numberOfPools++;
    }
    if (isOther(pools)) {
        numberOfPools++;
    }
    return numberOfPools;
}

bool canContainCharactersFromSelectedPools(int pools,
                                           const yapet::SecureArray& password) {
    int numberOfPools{countPools(pools)};
    int sizeOfPassword{password.size() - 1};

    return numberOfPools <= sizeOfPassword;
}

bool containsCharactersFromPool(const std::string& pool,
                                const yapet::SecureArray& password) {
    for (yapet::SecureArray::size_type i = 0; i < password.size() - 1; i++) {
        char passwordCharacter = static_cast<char>(password[i]);
        if (pool.find(passwordCharacter, 0) != std::string::npos) {
            return true;
        }
    }
    return false;
}
bool containsSelectedPools(int pools, const yapet::SecureArray& password) {
    if (!canContainCharactersFromSelectedPools(pools, password)) {
        return true;
    }

    bool satisfied = true;

    if (isLetters(pools)) {
        satisfied = satisfied &&
                    containsCharactersFromPool(yapet::pwgen::letters, password);
    }

    if (isDigits(pools)) {
        satisfied = satisfied &&
                    containsCharactersFromPool(yapet::pwgen::digits, password);
    }

    if (isPunct(pools)) {
        satisfied = satisfied && containsCharactersFromPool(
                                     yapet::pwgen::punctuation, password);
    }

    if (isSpecial(pools)) {
        satisfied = satisfied &&
                    containsCharactersFromPool(yapet::pwgen::special, password);
    }

    if (isOther(pools)) {
        satisfied = satisfied &&
                    containsCharactersFromPool(yapet::pwgen::other, password);
    }

    return satisfied;
}
}  // namespace

PasswordGenerator::PasswordGenerator(POOLS pool)
    : PasswordGenerator{static_cast<int>(pool)} {}

PasswordGenerator::PasswordGenerator(int pools) : _characterPools{pools} {}

PasswordGenerator::PasswordGenerator(PasswordGenerator&& other)
    : _characterPools{other._characterPools} {}

PasswordGenerator& PasswordGenerator::operator=(PasswordGenerator&& other) {
    if (this == &other) {
        return *this;
    }

    _characterPools = other._characterPools;

    return *this;
}

void PasswordGenerator::characterPools(int pools) { _characterPools = pools; }

yapet::SecureArray PasswordGenerator::generatePassword(int size) {
    if (size < 1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("%d is not a valid password size"), size);
        throw std::invalid_argument(msg);
    }

    std::string pools{getPools(_characterPools)};
    auto poolSize{pools.size()};

    assert(poolSize < std::numeric_limits<std::uint8_t>::max());
    std::uint8_t poolSizeUint8{static_cast<std::uint8_t>(poolSize)};

    Rng rng(std::uint8_t(poolSizeUint8) - 1);

    SecureArray password{size + 1};
    do {
        for (int i = 0; i < size; i++) {
            password[i] = pools[rng.getNextInt()];
        }
        password[size] = '\0';
    } while (!containsSelectedPools(_characterPools, password));
    return password;
}