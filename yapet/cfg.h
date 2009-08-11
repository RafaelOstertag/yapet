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

#ifndef _CFG_H
#define _CFG_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#include "cfgfile.h"

namespace YAPETCONFIG {

    /**
     * @brief Handle the command line and config file options.
     *
     * This class handles the command line and config file options.
     */
    class Config {
	private:
	    ConfigFile* cfgfile;

	    //! The default .pet file to open
	    static std::string def_petfile;
	    //! The default lock timeout
	    static int def_timeout;
	    //! Default for checking file security
	    static bool def_filesecurity;
	    //! Default for ignoring the rc file
	    static bool def_ignorerc;

	    struct s_cl_struct {
		    // For indicating whether or not it has been set on the command line
		    bool set_on_cl;
		    inline s_cl_struct() : set_on_cl(false) {}
		    inline s_cl_struct(const s_cl_struct& r) { set_on_cl = r.set_on_cl; }
		    inline const s_cl_struct& operator=(const s_cl_struct& r) {
			if (&r == this)
			    return *this;
			set_on_cl = r.set_on_cl;
			return *this;
		    }
	    };
	    //! The .pet file to open provided on the command line
	    struct s_cl_petfile : public s_cl_struct {
		    std::string name;
		    inline s_cl_petfile() : s_cl_struct(), name("") {}
		    inline s_cl_petfile(const s_cl_petfile& r) : s_cl_struct(r) { name = r.name ; }
		    inline const s_cl_petfile& operator=(const s_cl_petfile& r) {
			if (&r == this)
			    return *this;
			s_cl_struct::operator=(r);
			name = r.name;
			return *this;
		    }
	    };
	    //! The lock timeout provided on the command line
	    struct s_cl_timeout : public s_cl_struct {
		    unsigned int amount;
		    inline s_cl_timeout() : s_cl_struct(), amount(0) {}
		    inline s_cl_timeout(const s_cl_timeout& r) : s_cl_struct(r) { amount = r.amount ; }
		    inline const s_cl_timeout& operator=(const s_cl_timeout& r) {
			if (&r == this)
			    return *this;
			s_cl_struct::operator=(r);
			amount = r.amount;
			return *this;
		    }
	    };
	    //! The request for checking file security on the command line
	    struct s_cl_filesecurity : public s_cl_struct {
		    bool check;
		    inline s_cl_filesecurity() : s_cl_struct(), check(false) {}
		    inline s_cl_filesecurity(const s_cl_filesecurity& r) : s_cl_struct(r) { check = r.check ; }
		    inline const s_cl_filesecurity& operator=(const s_cl_filesecurity& r) {
			if (&r == this)
			    return *this;
			s_cl_struct::operator=(r);
			check = r.check;
			return *this;
		    }
	    };
	    //! Ignoring the rc file provided on the command line
	    struct s_cl_ignorerc : public s_cl_struct {
		    bool ignore;
		    inline s_cl_ignorerc() : s_cl_struct(), ignore(false) {}
		    inline s_cl_ignorerc(const s_cl_ignorerc& r) : s_cl_struct(r) { ignore = r.ignore ; }
		    inline const s_cl_ignorerc& operator=(const s_cl_ignorerc& r) {
			if (&r == this)
			    return *this;
			s_cl_struct::operator=(r);
			ignore = r.ignore;
			return *this;
		    }
	    };
	    s_cl_petfile cl_petfile;
	    s_cl_timeout cl_timeout;
	    s_cl_filesecurity cl_filesecurity;
	    s_cl_ignorerc cl_ignorerc;

	public:
	    static std::string getDefPetfile();
	    static unsigned int getDefTimeout();
	    static bool getDefFilesecurity();
	    static bool getDefIgnorerc();

	    Config();
	    Config(const Config& c);
	    ~Config();

	    void loadConfigFile(std::string filename="");

	    inline void setPetFile(std::string s) {
		cl_petfile.set_on_cl = true;
		cl_petfile.name = s;
	    }
	    inline void setTimeout(int i) {
		cl_timeout.set_on_cl = true;
		cl_timeout.amount = i;
	    }
	    inline void setFilesecurity(bool b) {
		cl_filesecurity.set_on_cl = true;
		cl_filesecurity.check = b;
	    }
	    inline void setIgnorerc(bool b) {
		cl_ignorerc.set_on_cl = true;
		cl_ignorerc.ignore = b;
	    }

	    const std::string& getPetFile() const;
	    int getTimeout() const;
	    bool getFilesecurity() const;

	    const Config& operator=(const Config& c);
    };

}

#endif // _CFG_H
