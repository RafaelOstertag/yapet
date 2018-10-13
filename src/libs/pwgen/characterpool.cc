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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "characterpool.hh"
#include "intl.h"

using namespace yapet::pwgen;

const std::string yapet::pwgen::letters{
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string yapet::pwgen::digits{"0123456789"};
const std::string yapet::pwgen::punctuation{".,;:-!?'"};
const std::string yapet::pwgen::special{"_+\"*%&/()[]={}<>"};
const std::string yapet::pwgen::other{"@#\\|$~`^"};

std::string yapet::pwgen::getPools(int pools) {
    std::string characterPools;
    if (isLetters(pools)) {
        characterPools += letters;
    }
    if (isDigits(pools)) {
        characterPools += digits;
    }
    if (isPunct(pools)) {
        characterPools += punctuation;
    }
    if (isSpecial(pools)) {
        characterPools += special;
    }
    if (isOther(pools)) {
        characterPools += other;
    }

    if (characterPools.size() == 0) {
        throw std::out_of_range{_("No character pools selected")};
    }

    return characterPools;
}