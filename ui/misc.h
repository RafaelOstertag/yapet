// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008,2009  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _MISC_H
#define _MISC_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
#include <string>
#endif

#if defined(HAVE_TERMINALTITLE) && defined(HAVE_TERMNAME)
#define CANSETTITLE
#endif

/**
 * @file
 * @brief Miscellaneous functions that do not fit elsewhere
 *
 * Miscellaneous functions that do not fit elsewhere
 */

#ifdef CANSETTITLE
    extern void setTerminalTitle (const std::string& title);
#endif


#endif // _MISC_H
