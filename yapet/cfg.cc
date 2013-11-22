// $Id$
//
// Copyright (C) 2009-2012  Rafael Ostertag
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef CFGDEBUG
#include <iostream>
#endif

#include <cctype>
#include <cstdlib>

#include "cfg.h"

using namespace YAPET::CONFIG;

//
// Class CfgValPetFile
//
std::string
CfgValPetFile::cleanup_path(const std::string& p) {
    if (p.find("//") == std::string::npos) return p;

    std::string::size_type pos=0;
    std::string working_copy(p);
    while ( (pos=working_copy.find("//")) != std::string::npos )
	working_copy = working_copy.replace(pos, 2, "/");

    return working_copy;
}
void
CfgValPetFile::set(const std::string& s) {
    if (s.empty()) {
	CfgValStr::set(s);
	return;
    }

    if (s.length() < 4) {
	// Since this holds, there can no ".pet"
	CfgValStr::set(s+YAPET::Consts::default_suffix);
	return;
    }

    // We don't have to check for the string length, because that
    // already happened above.
    //
    // s.length()-5 because to get the last four chars
    //      (s.length()-4)-1 = s.length()-5
    //
    if (s.substr(s.length()-5,4) != YAPET::Consts::default_suffix) {
	CfgValStr::set(s+YAPET::Consts::default_suffix);
	return;
    }

    CfgValStr::set(cleanup_path(s));

}

void
CfgValPetFile::set_str(const std::string& s) {
    set(s);
}
//
// Class CfgValBool
//
void
CfgValBool::set_str(const std::string& s) {
    std::string sanitized(tolower(remove_space(s)));

    if (sanitized == "0" ||
	sanitized == "false" ||
	sanitized == "no" ||
	sanitized == "disable" ||
	sanitized == "disabled") {
	set(false);
	return;
    }

    if (sanitized == "1" ||
	sanitized == "true" ||
	sanitized == "yes" ||
	sanitized == "enable" ||
	sanitized == "enabled") {
	set(false);
	return;
    }

    throw std::invalid_argument(sanitized + _(" is not a valid bool"));
}

//
// Class CfgValInt
//
void
CfgValInt::set_str(const std::string& s) {
    set(std::atoi(s.c_str()));
}

//
// Class CfgValRNG
//
void
CfgValRNG::set_str(const std::string& s) {
    std::string sanitized(tolower(remove_space(s)));

    if (sanitized == "devrandom") {
	set(YAPET::PWGEN::DEVRANDOM);
	return;
    }

    if (sanitized == "devurandom") {
	set(YAPET::PWGEN::DEVURANDOM);
	return;
    }

    if (sanitized == "lrand48") {
	set(YAPET::PWGEN::LRAND48);
	return;
    }

    if (sanitized == "rand") {
	set(YAPET::PWGEN::RAND);
	return;
    }

    if (sanitized == "auto") {
	set(YAPET::PWGEN::AUTO);
	return;
    }

    throw std::invalid_argument(sanitized + _(" is not a valid RNG"));
}

void
Config::setup_map() {
    __options.clear();

    __options["load"]=&petfile;
    __options["locktimeout"]=&timeout;
    __options["checkfsecurity"]=&filesecurity;
    __options["allowlockquit"]=&allow_lock_quit;
    __options["pwinputtimeout"]=&pw_input_timeout;
    __options["pwgen_rng"]=&pwgen_rng;
    __options["pwgen_pwlen"]=&pwgenpwlen;
    __options["pwgen_letters"]=&character_pools;
    __options["pwgen_digits"]=&character_pools;
    __options["pwgen_punct"]=&character_pools;
    __options["pwgen_special"]=&character_pools;
    __options["pwgen_other"]=&character_pools;
}



Config::Config() : petfile(std::string()),
		   timeout(-1,
			   YAPET::Consts::min_locktimeout,
			   YAPET::Consts::min_locktimeout),
		   filesecurity(YAPET::Consts::def_filesecurity),
		   pwgenpwlen(YAPET::Consts::def_pwlen,
			      YAPET::Consts::def_pwlen,
			      YAPET::Consts::min_pwlen,
			      YAPET::Consts::max_pwlen),
		   pwgen_rng(YAPET::Consts::def_pwgen_rng ),
		   character_pools(YAPET::Consts::def_character_pools),
		   allow_lock_quit(YAPET::Consts::def_allow_lock_quit),
		   pw_input_timeout(YAPET::Consts::def_pw_input_timeout,
				    YAPET::Consts::min_locktimeout,
				    YAPET::Consts::min_locktimeout) {
    setup_map();
}

Config::Config(const Config& c) :
    petfile(c.petfile),
    timeout(c.timeout),
    filesecurity(c.filesecurity),
    pwgenpwlen(c.pwgenpwlen),
    pwgen_rng(c.pwgen_rng),
    character_pools(c.character_pools),
    allow_lock_quit(c.allow_lock_quit),
    pw_input_timeout(c.pw_input_timeout) {
    setup_map();
}

Config::~Config() {
}

const Config&
Config::operator=(const Config& c) {
    if (&c == this)
	return *this;

    //
    // We don't need to (re)setup the map, because the pointers in the
    // map did not change!
    //

    petfile = c.petfile;
    timeout = c.timeout;
    filesecurity = c.filesecurity;
    pwgenpwlen = c.pwgenpwlen;
    pwgen_rng = c.pwgen_rng;
    character_pools = c.character_pools;
    allow_lock_quit = c.allow_lock_quit;
    pw_input_timeout = c.pw_input_timeout;

    return *this;
}

void
Config::lock() {
    petfile.lock();
    timeout.lock();
    filesecurity.lock();
    pwgenpwlen.lock();
    pwgen_rng.lock();
    character_pools.lock();
    allow_lock_quit.lock();
    pw_input_timeout.lock();
}

void
Config::unlock() {
    petfile.unlock();
    timeout.unlock();
    filesecurity.unlock();
    pwgenpwlen.unlock();
    pwgen_rng.unlock();
    character_pools.unlock();
    allow_lock_quit.unlock();
    pw_input_timeout.unlock();
}

CfgValBase&
Config::operator[](const std::string& key) {
    if (key.empty())
	throw std::invalid_argument(_("Configuration key must not be empty"));
    
    std::map<std::string,CfgValBase*>::iterator it =
	__options.find(key);

    if (it == __options.end())
	throw std::invalid_argument(std::string(_("Configuration key '")) + key + std::string(_("' not found")));

    return *((*it).second);
}
