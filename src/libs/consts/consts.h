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

/**
 * @file
 *
 * Header file for constant values.
 */

#ifndef _CONSTS_H
#define _CONSTS_H 1

#include <string>
// Used for the character pools
#include "characterpool.hh"
#include "rng.hh"

namespace YAPET {
class Consts {
   public:
    static const std::string ARGON2_TIME_COST_KEY;
    static const std::string ARGON2_MEMORY_COST_KEY;
    static const std::string ARGON2_PARALLELISM_KEY;
    static const std::string ARGON2_SALT1_KEY;
    static const std::string ARGON2_SALT2_KEY;
    static const std::string ARGON2_SALT3_KEY;
    static const std::string ARGON2_SALT4_KEY;
    //! Holds the default suffix for yapet files
    static const std::string DEFAULT_FILE_SUFFIX;
    //! The default file name of the config file
    static const std::string DEFAULT_RC_FILENAME;
    //! Default for checking file security
    static constexpr bool DEFAULT_FILE_SECURITY{true};
    static constexpr bool DEFAULT_ALLOW_LOCK_QUIT{true};

    //! Maximum password length
    static constexpr auto MAX_PASSWORD_LENGTH{256};
    //! Minimum password length
    static constexpr auto MIN_PASSWORD_LENGTH{2};
    //! The minimum lock timeout
    static constexpr auto MIN_LOCK_TIMEOUT{10};
    static constexpr auto MAX_CONFIG_LINE_LENGTH{1024};
    //! The default lock timeout
    static constexpr auto DEFAULT_LOCK_TIMEOUT{600};
    //! Default password length for Password Generator
    static constexpr auto DEFAULT_PASSWORD_LENGTH{15};
    static constexpr auto DEFAULT_PASSWORD_INPUT_TIMEOUT{60};
    static constexpr auto DEFAULT_CHARACTER_POOLS{
        yapet::pwgen::LETTERS | yapet::pwgen::DIGITS | yapet::pwgen::PUNCT |
        yapet::pwgen::SPECIAL};

    // Argon2 options

    // in kibi
    static constexpr int DEFAULT_ARGON2_MEMORY{262144};
    static constexpr int MIN_ARGON2_MEMORY{65536};
    // threads
    static constexpr int DEFAULT_ARGON2_PARALLELISM{16};
    static constexpr int MIN_ARGON2_PARALLELISM{1};
    // iterations
    static constexpr int DEFAULT_ARGON2_TIME_COST{5};
    static constexpr int MIN_ARGON2_TIME_COSTS{2};

    static constexpr auto EXCEPTION_MESSAGE_BUFFER_SIZE{512};
};
}  // namespace YAPET
#endif  // _CONSTS_H
