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
# include <config.h>
#endif

#ifdef CFGDEBUG
# ifdef HAVE_IOSTREAM
#  include <iostream>
# endif
#endif

#include "cfg.h"
// Used for the character pools
#include "pwgen/pwgen.h"

using namespace YAPET::CONFIG;
//
// Global
//
Config YAPET::CONFIG::config;

//! The default .pet file to open
const std::string Config::def_petfile("");
//! The default lock timeout
const int Config::def_timeout(600);
//! Default for checking file security
const bool Config::def_filesecurity(true);
//! Default for ignoring the rc file
const bool Config::def_ignorerc(false);
const YAPET::PWGEN::RNGENGINE Config::def_pwgen_rng(YAPET::PWGEN::AUTO);
const size_t Config::def_pwlen(10);
const int Config::def_character_pools(YAPET::PWGEN::LETTERS |
                                      YAPET::PWGEN::DIGITS |
                                      YAPET::PWGEN::PUNCT |
                                      YAPET::PWGEN::SPECIAL);
const bool Config::def_allow_lock_quit(true);
const unsigned int Config::def_pw_input_timeout(60);  // in seconds
std::string
Config::getDefPetfile() {
    return def_petfile;
}

unsigned int
Config::getDefTimeout() {
    return def_timeout;
}

bool
Config::getDefFilesecurity() {
    return def_filesecurity;
}

bool
Config::getDefIgnorerc() {
    return def_ignorerc;
}

YAPET::PWGEN::RNGENGINE
Config::getDefPWGenRNG() {
    return def_pwgen_rng;
}

size_t
Config::getDefPWLength() {
    return def_pwlen;
}

int
Config::getDefCharPools() {
    return def_character_pools;
}

bool
Config::getDefAllowLockQuit() {
    return def_allow_lock_quit;
}

unsigned int
Config::getDefPwInputTimeout() {
    return def_pw_input_timeout;
}

bool
Config::getDefCPoolLetters() {
    return def_character_pools & YAPET::PWGEN::LETTERS ? true : false;
}

bool
Config::getDefCPoolDigits() {
    return def_character_pools & YAPET::PWGEN::DIGITS ? true : false;
}

bool
Config::getDefCPoolPunct() {
    return def_character_pools & YAPET::PWGEN::PUNCT ? true : false;
}

bool
Config::getDefCPoolSpecial() {
    return def_character_pools & YAPET::PWGEN::SPECIAL ? true : false;
}

bool
Config::getDefCPoolOther() {
    return def_character_pools & YAPET::PWGEN::OTHER ? true : false;
}

std::string
Config::cleanupPath(const std::string& s) const {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::cleanupPath(std::string)";
    std::cout << ":" << std::endl;
#endif

    if (s.empty() ) {
#ifdef CFGDEBUG
        std::cout << "\tgot empty string." << std::endl;
#endif
        return s;
    }

    std::string work_copy(s);
#ifdef CFGDEBUG
    std::cout << "\tBefore cleanup: " << s << std::endl;
#endif
    std::string::size_type pos;

    while ( (pos = work_copy.find("//", 0) ) != std::string::npos)
        work_copy.erase(pos, 1);

#ifdef CFGDEBUG
    std::cout << "\tAfter cleanup: " << work_copy << std::endl;
#endif
    return work_copy;
}

Config::Config() : petfile(""),
    timeout(Config::getDefTimeout() ),
    filesecurity(Config::getDefFilesecurity() ),
    ignorerc(Config::getDefIgnorerc() ),
    pwgenpwlen(Config::getDefPWLength() ),
    pwgen_rng(Config::getDefPWGenRNG() ),
    character_pools(Config::getDefCharPools() ),
    allow_lock_quit(Config::getDefAllowLockQuit() ),
    pw_input_timeout(Config::getDefPwInputTimeout() ) {
    // Empty
}

Config::Config(const Config& c) {
    petfile = c.petfile;
    timeout = c.timeout;
    filesecurity = c.filesecurity;
    ignorerc = c.ignorerc;
    pwgenpwlen = c.pwgenpwlen;
    pwgen_rng = c.pwgen_rng;
    character_pools = c.character_pools;
    allow_lock_quit = c.allow_lock_quit;
    pw_input_timeout = c.pw_input_timeout;
}

Config::~Config() {
}

/**
 * @param filename the file name to load. If empty, the default file will be loaded
 */
void
Config::loadConfigFile(std::string filename) {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::loadConfigFile(std::string)";
    std::cout << ":" << std::endl;
#endif

    if (ignorerc.get() ) {
#ifdef CFGDEBUG
        std::cout << "\tadvised to ignore rc file!" << std::endl;
#endif
        return;
    }

    ConfigFile cfgfile(filename);

    if (!cfgfile.isOpenSuccess() ) {
#ifdef CFGDEBUG
        std::cout << "\topen " << filename << " was not successful" <<
        std::endl;
#endif
    }

    ignorerc = cfgfile.getIgnoreRC();

    if (ignorerc.get() ) {
#ifdef CFGDEBUG
        std::cout << "\tRC file says to ignore itself!" << std::endl;
#endif
        return;
    }

    // we use this method because it cleans up the file path
    setPetFile(cfgfile.getFileToLoad() );

    filesecurity = cfgfile.getUseFileSecurity();
    timeout = cfgfile.getLockTimeout();
    pw_input_timeout = cfgfile.getPwInputTimeout();
    allow_lock_quit = cfgfile.getAllowLockQuit();
    pwgen_rng = cfgfile.getPWGenRNG();
    pwgenpwlen = cfgfile.getPWGenPWLen();
    character_pools = cfgfile.getCharPools();
}

const Config&
Config::operator=(const Config& c) {
    if (&c == this)
        return *this;

    petfile = c.petfile;
    timeout = c.timeout;
    filesecurity = c.filesecurity;
    ignorerc = c.ignorerc;
    pwgenpwlen = c.pwgenpwlen;
    pwgen_rng = c.pwgen_rng;
    character_pools = c.character_pools;
    allow_lock_quit = c.allow_lock_quit;
    pw_input_timeout = c.pw_input_timeout;

    return *this;
}
