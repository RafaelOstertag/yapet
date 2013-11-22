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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <unistd.h>

#include <pwd.h>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>

#ifdef CFGDEBUG
# include <iostream>
#endif

#include "consts.h"
#include "cfgfile.h"
#include "cfg.h"

using namespace YAPET::CONFIG;

std::string
ConfigFile::getHomeDir() const {
    std::string homedir("");

    char* hd = getenv("HOME");

    if (hd != NULL) {
        homedir = hd;

        if (homedir[homedir.length() ] != '/')
            homedir.push_back('/');

        return homedir;
    }

    struct passwd* pwd;
    pwd = getpwuid(getuid() );

    if (pwd != NULL) {
        homedir = pwd->pw_dir;

        if (homedir[homedir.length() ] != '/')
            homedir.push_back('/');

        return homedir;
    }

    assert(!homedir.empty() );
    return homedir;
}

void
ConfigFile::parseFile() {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "ConfigFile::parseFile()";
    std::cout << ":" << std::endl;
#endif

    try {
        std::ifstream cfgsin(cfgfilepath.c_str() );

        if (!cfgsin)
            return;

        const int MAX_LENGTH = 1024;
        char line[MAX_LENGTH];

        while (cfgsin.getline(line, MAX_LENGTH) ) {
            std::string l(line);

            ReadResult res;
            if ( (res =
                      readOption<std::string>(l, "load=",
                                              filetoload) ) !=
                 OPTION_NOT_FOUND) {
                if (res == OPTION_EMPTY)
                    continue;

                // Append the suffix if necessary
                if (filetoload.find(YAPET::CONSTS::Consts::getDefaultSuffix(),
                                    filetoload.length() -
                                    YAPET::CONSTS::Consts::getDefaultSuffix().
                                    length() )
                    == std::string::npos)
                    filetoload += YAPET::CONSTS::Consts::getDefaultSuffix();

                // Check if we have to replace the ~. It will only be replaced
                // if it is the first character of the file to load.
                if (filetoload.at(0) == '~') {
#ifdef CFGDEBUG
                    std::cout << "\tReplace ~ in load option by " <<
                        getHomeDir() << std::endl;
#endif
                    filetoload.erase(0, 1);
                    filetoload = getHomeDir() + filetoload;
                }

                continue;
            }

            if (readOption<unsigned int>(l, "locktimeout=",
                                         locktimeout) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "checkfsecurity=",
                                 usefsecurity) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "allowlockquit=",
                                 allowlockquit) != OPTION_NOT_FOUND)
                continue;

            if (readOption<unsigned int>(l, "pwinputtimeout=",
                                         pwinputtimeout) != OPTION_NOT_FOUND)
                continue;

            std::string tmp;
            if ( (res =
                      readOption<std::string>(l, "pwgen_rng=",
                                              tmp) ) != OPTION_NOT_FOUND) {
                if (res == OPTION_EMPTY)
                    continue;

                if (tmp == "devrandom") {
                    pwgen_rng = YAPET::PWGEN::DEVRANDOM;
                    continue;
                }

                if (tmp == "devurandom") {
                    pwgen_rng = YAPET::PWGEN::DEVURANDOM;
                    continue;
                }

                if (tmp == "lrand48") {
                    pwgen_rng = YAPET::PWGEN::LRAND48;
                    continue;
                }

                if (tmp == "rand") {
                    pwgen_rng = YAPET::PWGEN::RAND;
                    continue;
                }

                pwgen_rng = YAPET::PWGEN::AUTO;
                continue;
            }

            if (readOption<size_t>(l, "pwgen_pwlen=",
                                   pwgen_pwlen) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "pwgen_letters=",
                                 pwgen_letters) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "pwgen_digits=",
                                 pwgen_digits) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "pwgen_punct=",
                                 pwgen_punct) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "pwgen_special=",
                                 pwgen_special) != OPTION_NOT_FOUND)
                continue;

            if (readOption<bool>(l, "pwgen_other=",
                                 pwgen_other) != OPTION_NOT_FOUND)
                continue;
        }

        cfgsin.close();
    } catch (...) {
        // lazy me
    }
}

ConfigFile::ConfigFile(std::string cfgfile) : filetoload(
        Config::getDefPetfile() ),
    usefsecurity(Config::getDefFilesecurity() ),
    locktimeout(Config::getDefTimeout() ),
    pwinputtimeout(Config::getDefPwInputTimeout() ),
    allowlockquit(Config::getDefAllowLockQuit() ),
    cfgfilepath(""),
    opensuccess(true),
    pwgen_letters(Config::getDefCPoolLetters() ),
    pwgen_digits(Config::getDefCPoolDigits() ),
    pwgen_punct(Config::getDefCPoolPunct() ),
    pwgen_special(Config::getDefCPoolSpecial() ),
    pwgen_other(Config::getDefCPoolOther() ),
    pwgen_rng(Config::getDefPWGenRNG() ),
    pwgen_pwlen(Config::getDefPWLength() ) {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "ConfigFile::ConfigFile(std::string)";
    std::cout << ":" << std::endl;
#endif

    if (cfgfile.empty() ) {
        cfgfilepath = getHomeDir() +
                      YAPET::CONSTS::Consts::getDefaultRCFilename();
#ifdef CFGDEBUG
        std::cout << "\tcfgfile.empty() == true" << std::endl;
        std::cout << "\tcfgfilepath: " << cfgfilepath << std::endl;
#endif
    } else {
        cfgfilepath = cfgfile;
#ifdef CFGDEBUG
        std::cout << "\tcfgfile.empty() == false" << std::endl;
        std::cout << "\tcfgfilepath = cfgfile: " << cfgfilepath << std::endl;
#endif
    }

    if (access(cfgfilepath.c_str(), R_OK | F_OK) == -1) {
#ifdef CFGDEBUG
        std::cout << "\taccess to " << cfgfilepath << " denied." << std::endl;
#endif
        cfgfilepath.clear();
        opensuccess = false;
        return;
    }

    parseFile();
}

ConfigFile::ConfigFile(const ConfigFile& cfgfile) :
    filetoload(cfgfile.filetoload),
    usefsecurity(cfgfile.usefsecurity),
    locktimeout(cfgfile.locktimeout),
    pwinputtimeout(cfgfile.pwinputtimeout),
    allowlockquit(cfgfile.allowlockquit),
    cfgfilepath(cfgfile.cfgfilepath),
    opensuccess(cfgfile.opensuccess),
    pwgen_letters(cfgfile.pwgen_letters),
    pwgen_digits(cfgfile.pwgen_digits),
    pwgen_punct(cfgfile.pwgen_punct),
    pwgen_special(cfgfile.pwgen_special),
    pwgen_other(cfgfile.pwgen_other),
    pwgen_rng(cfgfile.pwgen_rng),
    pwgen_pwlen(cfgfile.pwgen_pwlen) {
}

const ConfigFile&
ConfigFile::operator=(const ConfigFile& cfgfile) {
    if (&cfgfile == this)
        return *this;

    filetoload = cfgfile.filetoload;
    usefsecurity = cfgfile.usefsecurity;
    locktimeout = cfgfile.locktimeout;
    pwinputtimeout = cfgfile.pwinputtimeout;
    allowlockquit = cfgfile.allowlockquit;
    cfgfilepath = cfgfile.cfgfilepath;
    opensuccess = cfgfile.opensuccess;
    pwgen_letters = cfgfile.pwgen_letters;
    pwgen_digits = cfgfile.pwgen_digits;
    pwgen_punct = cfgfile.pwgen_punct;
    pwgen_special = cfgfile.pwgen_special;
    pwgen_other = cfgfile.pwgen_other;
    pwgen_rng = cfgfile.pwgen_rng;
    pwgen_pwlen = cfgfile.pwgen_pwlen;

    return *this;
}
