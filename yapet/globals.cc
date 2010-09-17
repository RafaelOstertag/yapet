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

#include "globals.h"
#include "cfg.h"
#include "consts.h"

using namespace YAPET::GLOBALS;

YAPET::PWGEN::RNGENGINE Globals::pwgen_rng(YAPET::CONFIG::Config::getDefPWGenRNG());
int Globals::selected_character_pools(YAPET::CONFIG::Config::getDefCharPools());
size_t Globals::selected_password_length(YAPET::CONFIG::Config::getDefPWLength());

void
Globals::setPWGenRNG(YAPET::PWGEN::RNGENGINE e) {
    pwgen_rng = e;
}

YAPET::PWGEN::RNGENGINE
Globals::getPWGenRNG() {
    return pwgen_rng;
}

void
Globals::setCharacterPools(int cp) {
    selected_character_pools = cp;
}

int
Globals::getCharacterPools() {
    return selected_character_pools;
}

void
Globals::setPasswordLength(size_t pl) {
    selected_password_length = pl;
}

size_t
Globals::getPasswordLength() {
    return selected_password_length;
}
