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
#include "pwgen.h"

namespace YAPET {
class Consts {
   public:
    //! Holds the default suffix for yapet files
    static const std::string default_suffix;
    //! The default file name of the config file
    static const std::string default_rcfilename;
    //! Default for checking file security
    static const bool def_filesecurity;
    static const bool def_allow_lock_quit;
    static const YAPET::PWGEN::RNGENGINE def_pwgen_rng;

    enum {
        //! Maximum password length
        max_pwlen = 256,
        //! Maximum password length
        min_pwlen = 2,
        //! The minimum lock timeout
        min_locktimeout = 10,
        max_config_line_length = 1024,
        //! The default lock timeout
        def_locktimeout = 600,
        //! Default password length for Password Generator
        def_pwlen = 15,
        def_pw_input_timeout = 60,
        def_character_pools = YAPET::PWGEN::LETTERS | YAPET::PWGEN::DIGITS |
                              YAPET::PWGEN::PUNCT | YAPET::PWGEN::SPECIAL
    };
};
}  // namespace YAPET
#endif  // _CONSTS_H
