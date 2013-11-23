// $Id$
//
// Copyright (C) 2009-2013  Rafael Ostertag
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

using namespace YAPET;
using namespace YAPET::CONFIG;

namespace YAPET {
    namespace CONFIG {
	std::string
	trim(const std::string& s) {
	    if (s.empty()) return s;
	    
	    // find leading spaces
	    std::string::size_type pos=0;
	    while(std::isspace(s[pos++]) && s.length() > pos);
	    pos--;
	    assert(pos >= 0 && pos < s.length());
	    
	    std::string working_copy(s.substr(pos));
	    
	    // find trailing spaces
	    pos = working_copy.length()-1;
	    while(std::isspace(working_copy[pos--]) && pos >= 0);
	    pos++;
	    assert(pos>=0 && pos<=working_copy.length());
	    
	    return working_copy.substr(0, pos+1);
	}
    }
}

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

std::string
CfgValPetFile::add_suffix(const std::string& p) {
    if (p.empty()) {
	return p;
    }

    if (p.length() < 4) {
	// Since this holds, there can no ".pet"
	return p+Consts::default_suffix;
    }

    // We don't have to check for the string length, because that
    // already happened above.
    if (p.substr(p.length()-4,4) != Consts::default_suffix) {
	return p+Consts::default_suffix;
    }

    return p;
}

void
CfgValPetFile::set(const std::string& s) {
    CfgValStr::set(add_suffix(cleanup_path(s)));
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
    std::string sanitized(tolower(trim(s)));

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
	set(true);
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
	set(PWGEN::DEVRANDOM);
	return;
    }

    if (sanitized == "devurandom") {
	set(PWGEN::DEVURANDOM);
	return;
    }

    if (sanitized == "lrand48") {
	set(PWGEN::LRAND48);
	return;
    }

    if (sanitized == "rand") {
	set(PWGEN::RAND);
	return;
    }

    if (sanitized == "auto") {
	set(PWGEN::AUTO);
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
    __options["pwgen_letters"]=&pwgen_letters;
    __options["pwgen_digits"]=&pwgen_digits;
    __options["pwgen_punct"]=&pwgen_punct;
    __options["pwgen_special"]=&pwgen_special;
    __options["pwgen_other"]=&pwgen_other;
}



Config::Config() : 
    petfile(std::string()),
    timeout(Consts::def_locktimeout,
	    Consts::min_locktimeout,
	    Consts::min_locktimeout),
    filesecurity(Consts::def_filesecurity),
    pwgenpwlen(Consts::def_pwlen,
	       Consts::def_pwlen,
	       Consts::min_pwlen,
	       Consts::max_pwlen),
    pwgen_rng(Consts::def_pwgen_rng ),
    pwgen_letters(PWGEN::HAS_LETTERS(Consts::def_character_pools)),
    pwgen_digits(PWGEN::HAS_DIGITS(Consts::def_character_pools)),
    pwgen_punct(PWGEN::HAS_PUNCT(Consts::def_character_pools)),
    pwgen_special(PWGEN::HAS_SPECIAL(Consts::def_character_pools)),
    pwgen_other(PWGEN::HAS_OTHER(Consts::def_character_pools)),
    allow_lock_quit(Consts::def_allow_lock_quit),
    pw_input_timeout(Consts::def_pw_input_timeout,
		     Consts::min_locktimeout,
		     Consts::min_locktimeout) {
    setup_map();
}

Config::Config(const Config& c) :
    petfile(c.petfile),
    timeout(c.timeout),
    filesecurity(c.filesecurity),
    pwgenpwlen(c.pwgenpwlen),
    pwgen_rng(c.pwgen_rng),
    pwgen_letters(c.pwgen_letters),
    pwgen_digits(c.pwgen_digits),
    pwgen_punct(c.pwgen_punct),
    pwgen_special(c.pwgen_special),
    pwgen_other(c.pwgen_other),
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
    pwgen_letters = c.pwgen_letters;
    pwgen_digits = c.pwgen_digits;
    pwgen_punct = c.pwgen_punct;
    pwgen_special = c.pwgen_special;
    pwgen_other = c.pwgen_other;
    allow_lock_quit = c.allow_lock_quit;
    pw_input_timeout = c.pw_input_timeout;

    return *this;
}

int
Config::character_pools() const {
    int pools=0;

    if (pwgen_letters)
	pools|=PWGEN::LETTERS;

    if (pwgen_digits)
	pools|=PWGEN::DIGITS;

    if (pwgen_punct)
	pools|=PWGEN::PUNCT;

    if (pwgen_special)
	pools|=PWGEN::SPECIAL;

    if (pwgen_other)
	pools|=PWGEN::OTHER;

    return pools;
}

void
Config::lock() {
    petfile.lock();
    timeout.lock();
    filesecurity.lock();
    pwgenpwlen.lock();
    pwgen_rng.lock();
    pwgen_letters.lock();
    pwgen_digits.lock();
    pwgen_punct.lock();
    pwgen_special.lock();
    pwgen_other.lock();
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
    pwgen_letters.unlock();
    pwgen_digits.unlock();
    pwgen_punct.unlock();
    pwgen_special.unlock();
    pwgen_other.unlock();
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
