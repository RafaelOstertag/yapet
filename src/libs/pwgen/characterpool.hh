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

#ifndef _CHARACTERPOOL_HH
#define _CHARACTERPOOL_HH

#include <string>
#include <vector>

namespace yapet {
namespace pwgen {

/**
 * @brief Available character pools
 *
 * Pools to choose from.
 */
enum POOLS {
    /**
     * Letters: a-z, A-Z
     */
    LETTERS = (1 << 0),
    /**
     * Digits: 0-9
     */
    DIGITS = (1 << 1),
    /**
     * Punctuation characters: comma, semicolon, etc.
     */
    PUNCT = (1 << 2),
    /**
     * Special characters: #,(,), etc.
     */
    SPECIAL = (1 << 3),
    /**
     * Other characters
     */
    OTHER = (1 << 4),
    ALL = LETTERS | DIGITS | PUNCT | SPECIAL | OTHER
};

constexpr bool isLetters(int v) { return (v & LETTERS); }
constexpr bool isDigits(int v) { return (v & DIGITS); }
constexpr bool isPunct(int v) { return (v & PUNCT); }
constexpr bool isSpecial(int v) { return (v & SPECIAL); }
constexpr bool isOther(int v) { return (v & OTHER); }
constexpr bool isAll(int v) { return (v & ALL); }

extern const std::string letters;
extern const std::string digits;
extern const std::string punctuation;
extern const std::string special;
extern const std::string other;

std::string getPools(int pools);

}  // namespace pwgen
}  // namespace yapet

#endif