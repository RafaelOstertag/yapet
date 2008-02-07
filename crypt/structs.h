// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
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

#ifndef _STRUCTS_H
#define _STRUCTS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

namespace YAPET {

    enum {
	HEADER_CONTROL_SIZE = 20,
	NAME_SIZE = 128,
	HOST_SIZE = 256,
	USERNAME_SIZE = 256,
	PASSWORD_SIZE = 256,
	COMMENT_SIZE = 512
    };

    struct FileHeader {
	    uint8_t version;
	    uint8_t control[HEADER_CONTROL_SIZE];
	    time_t pwset;
    };

    struct PasswordRecord {
	    uint8_t name[NAME_SIZE];
	    uint8_t host[HOST_SIZE];
	    uint8_t username[USERNAME_SIZE];
	    uint8_t password[PASSWORD_SIZE];
	    uint8_t comment[COMMENT_SIZE];
    };
}

#endif // _STRUCTS_H
