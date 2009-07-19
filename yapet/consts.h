// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
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
#define _CONSTS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

class Consts {
    private:
	//! Holds the default suffix for yapet files
	static const std::string default_suffix;
	inline Consts() {}
	inline ~Consts() {}
	inline Consts(const Consts&) {}
	inline const Consts& operator=(const Consts&) { return *this; }

    public:
	//! Return the default suffix for yapet files
	static const std::string& getDefaultSuffix();
};

#endif // _CONSTS_H
