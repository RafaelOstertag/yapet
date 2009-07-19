// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_FSTREAM
#include <fstream>
#endif

#ifdef HAVE_SSTREAM
#include <sstream>
#endif

#include "consts.h"
#include "cfgfile.h"

std::string ConfigFile::cfgfile(".yapet");
std::string ConfigFile::getCfgFile() { return cfgfile; }

std::string
ConfigFile::getHomeDir() const {
    std::string homedir("");
#ifdef HAVE_GETENV
    char* hd = getenv("HOME");
    if (hd != NULL) {
	homedir = hd;
	if (homedir[homedir.length()] != '/')
	    homedir.push_back('/');
	return homedir;
    }
#endif
#if defined(HAVE_GETPWUID) && defined (HAVE_GETUID)
    struct passwd* pwd;

    pwd = getpwuid(getuid());
    if (pwd != NULL) {
	homedir = pwd->pw_dir;
	if (homedir[homedir.length()] != '/')
	    homedir.push_back('/');
	return homedir;
    }
#endif
	return homedir;
}

void
ConfigFile::parseFile() {
    try {
	std::ifstream cfgsin(cfgfilepath.c_str());
	if (!cfgsin)
	    return;

	const int MAX_LENGTH = 1024;
	char line[MAX_LENGTH];

	while (cfgsin.getline(line, MAX_LENGTH) ) {
	    std::string l(line);
	    std::string needle("load=");
	    if (l.find(needle,0) == 0) {
		l.erase(0,needle.length());
		filetoload = l;
		if ( filetoload.find(Consts::getDefaultSuffix(),
				     filetoload.length() - Consts::getDefaultSuffix().length())
		     == std::string::npos )
		    filetoload += Consts::getDefaultSuffix();
		continue;
	    }
	    needle = "locktimeout=";
	    if (l.find(needle,0) == 0) {
		l.erase(0,needle.length());
		std::istringstream sstr(l);
		sstr >> locktimeout;
		continue;
	    }
	    needle = "checkfsecurity=";
	    if (l.find(needle,0) == 0) {
		l.erase(0,needle.length());
		std::istringstream sstr(l);
		sstr >> usefsecurity;
		continue;
	    }
	}

	cfgsin.close();
    } catch (...) {
	// lazy me
    }
}


ConfigFile::ConfigFile() : filetoload(""),
			   usefsecurity(true),
			   locktimeout(600),
			   cfgfilepath("") {
    cfgfilepath = getHomeDir() + ConfigFile::getCfgFile();

    if (access(cfgfilepath.c_str(), R_OK | F_OK) == -1) {
	cfgfilepath.clear();
	return;
    }
    parseFile();
}
