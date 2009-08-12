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
# include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_PWD_H
# include <pwd.h>
#endif

#ifdef HAVE_FSTREAM
# include <fstream>
#endif

#ifdef HAVE_SSTREAM
# include <sstream>
#endif

#ifdef CFGDEBUG
# ifdef HAVE_IOSTREAM
#  include <iostream>
# endif
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#include "consts.h"
#include "cfgfile.h"
#include "cfg.h"

using namespace YAPETCONFIG;

std::string
ConfigFile::getHomeDir() const {
    std::string homedir ("");
#ifdef HAVE_GETENV
    char* hd = getenv ("HOME");

    if (hd != NULL) {
	homedir = hd;

	if (homedir[homedir.length() ] != '/')
	    homedir.push_back ('/');

	return homedir;
    }

#endif
#if defined(HAVE_GETPWUID) && defined (HAVE_GETUID)
    struct passwd* pwd;
    pwd = getpwuid (getuid() );

    if (pwd != NULL) {
	homedir = pwd->pw_dir;

	if (homedir[homedir.length() ] != '/')
	    homedir.push_back ('/');

	return homedir;
    }

#endif
    assert (!homedir.empty() );
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
	std::ifstream cfgsin (cfgfilepath.c_str() );

	if (!cfgsin)
	    return;

	const int MAX_LENGTH = 1024;
	char line[MAX_LENGTH];

	while (cfgsin.getline (line, MAX_LENGTH) ) {
	    std::string l (line);
	    std::string needle ("load=");

	    if (l.find (needle, 0) == 0) {
		l.erase (0, needle.length() );

		// Bail out if l is empty
		if (l.empty() )
		    continue;

		filetoload = l;

		if ( filetoload.find (YAPETCONSTS::Consts::getDefaultSuffix(),
				      filetoload.length() -
				      YAPETCONSTS::Consts::getDefaultSuffix().length() )
			== std::string::npos )
		    filetoload += YAPETCONSTS::Consts::getDefaultSuffix();

		// Check if we have to replace the ~. It will only be replaced
		// if it is the first character of the file to load.
		if (filetoload.at (0) == '~') {
#ifdef CFGDEBUG
		    std::cout << "\tReplace ~ in load option by " <<
			      getHomeDir() << std::endl;
#endif
		    filetoload.erase (0, 1);
		    filetoload = getHomeDir() + filetoload;
		}

		continue;
	    }

	    needle = "locktimeout=";

	    if (l.find (needle, 0) == 0) {
		l.erase (0, needle.length() );

		// Bail out if l is empty
		if (l.empty() )
		    continue;

		std::istringstream sstr (l);
		sstr >> locktimeout;
		continue;
	    }

	    needle = "checkfsecurity=";

	    if (l.find (needle, 0) == 0) {
		l.erase (0, needle.length() );

		// Bail out if l is empty
		if (l.empty() )
		    continue;

		std::istringstream sstr (l);
		sstr >> usefsecurity;
		continue;
	    }

	    // Yes, the file can say that it should be ignored!
	    needle = "ignorerc=";

	    if (l.find (needle, 0) == 0) {
		l.erase (0, needle.length() );

		// Bail out if l is empty
		if (l.empty() )
		    continue;

		std::istringstream sstr (l);
		sstr >> ignorerc;
		continue;
	    }
	}

	cfgsin.close();
    } catch (...) {
	// lazy me
    }
}


ConfigFile::ConfigFile (std::string cfgfile) : filetoload (Config::getDefPetfile() ),
	usefsecurity (Config::getDefFilesecurity() ),
	locktimeout (Config::getDefTimeout() ),
	ignorerc (false),
	cfgfilepath (""),
	opensuccess (true) {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "ConfigFile::ConfigFile(std::string)";
    std::cout << ":" << std::endl;
#endif

    if (cfgfile.empty() ) {
	cfgfilepath = getHomeDir() +
		      YAPETCONSTS::Consts::getDefaultRCFilename();
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

    if (access (cfgfilepath.c_str(), R_OK | F_OK) == -1) {
#ifdef CFGDEBUG
	std::cout << "\taccess to " << cfgfilepath << " denied." << std::endl;
#endif
	cfgfilepath.clear();
	opensuccess = false;
	return;
    }

    parseFile();
}

ConfigFile::ConfigFile (const ConfigFile& cfgfile) {
    filetoload = cfgfile.filetoload;
    usefsecurity = cfgfile.usefsecurity;
    locktimeout = cfgfile.locktimeout;
    cfgfilepath = cfgfile.cfgfilepath;
}

const ConfigFile&
ConfigFile::operator= (const ConfigFile & cfgfile) {
    if (&cfgfile == this)
	return *this;

    filetoload = cfgfile.filetoload;
    usefsecurity = cfgfile.usefsecurity;
    locktimeout = cfgfile.locktimeout;
    cfgfilepath = cfgfile.cfgfilepath;
    return *this;
}
