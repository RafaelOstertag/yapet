// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#ifndef _MISC_H
#define _MISC_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
#include <string>
#endif

/**
 * @file
 * @brief Miscellaneous functions that do not fit elsewhere
 *
 * Miscellaneous functions that do not fit elsewhere
 */

/** @brief Indicates whether or not the terminal is an X Terminal */
extern bool isXTerm();

/** @brief Set the title on a terminal */
extern void setTerminalTitle (const std::string& title);


#endif // _MISC_H
