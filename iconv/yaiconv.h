// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2014  Rafael Ostertag
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

#ifndef _YAICONV_H
#define _YAIONV_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cerrno>
#include <stdexcept>
#include <cstdlib>
#include <string>

#include <iconv.h>

namespace YAPET {
    class IconvException : public std::runtime_error {
	private:
	    int __errno;
	    std::string __errmsg;

	public:
	    IconvException(int _errno, const std::string& _errmsg):
		__errno(__errno),
		__errmsg(_errmsg) {}

	    IconvException(const IconvException& _ref):
		__errno(_ref.__errno),
		__errmsg(_ref.__errmsg) {}

	    const char* what() const {
		return __errmsg.c_str();
	    }

	    bool operator==(int val) const {
		return __errno == val;
	    }
    };

    class Iconv {
	private:
	    std::string __fromcode;
	    std::string __tocode;

	    Iconv(const Iconv& _o) { std::abort(); }
	    const Iconv& operator=(const Iconv& _o) { std::abort(); return *this };

	public:
	    Iconv(const std::string& from, const std::string& to);

	    std::string operator()(const std::string& _in) const;
}

#endif // _CFGFILE_H
