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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef CFGDEBUG
# ifdef HAVE_IOSTREAM
#  include <iostream>
# endif
#endif

#include "cfg.h"

using namespace YAPET::CONFIG;

//! The default .pet file to open
std::string Config::def_petfile ("");
//! The default lock timeout
int Config::def_timeout (600);
//! Default for checking file security
bool Config::def_filesecurity (true);
//! Default for ignoring the rc file
bool Config::def_ignorerc (false);

std::string Config::getDefPetfile() {
    return def_petfile;
}
unsigned int Config::getDefTimeout() {
    return def_timeout;
}
bool Config::getDefFilesecurity() {
    return def_filesecurity;
}
bool Config::getDefIgnorerc() {
    return def_ignorerc;
}

std::string
Config::cleanupPath (const std::string& s) const {
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

    std::string work_copy (s);
#ifdef CFGDEBUG
    std::cout << "\tBefore cleanup: " << s << std::endl;
#endif
    std::string::size_type pos;

    while ( (pos = work_copy.find ("//", 0) ) != std::string::npos)
	work_copy.erase (pos, 1);

#ifdef CFGDEBUG
    std::cout << "\tAfter cleanup: " << work_copy << std::endl;
#endif
    return work_copy;
}

Config::Config() : cfgfile (NULL),
	cl_petfile(),
	cl_timeout(),
	cl_filesecurity(),
	cl_ignorerc() {
    // Empty
}

Config::Config (const Config& c) {
    if (c.cfgfile != NULL)
	cfgfile = new ConfigFile (* (c.cfgfile) );
    else
	cfgfile = NULL;

    cl_petfile = c.cl_petfile;
    cl_timeout = c.cl_timeout;
    cl_filesecurity = c.cl_filesecurity;
    cl_ignorerc = c.cl_ignorerc;
}

Config::~Config() {
    if (cfgfile != NULL)
	delete cfgfile;
}

/**
 * @param filename the file name to load. If empty, the default file will be loaded
 */
void
Config::loadConfigFile (std::string filename) {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::loadConfigFile(std::string)";
    std::cout << ":" << std::endl;
#endif

    if (cl_ignorerc.ignore) {
#ifdef CFGDEBUG
	std::cout << "\tadvised to ignore rc file!" << std::endl;
#endif

	if (cfgfile != NULL) {
	    delete cfgfile;
	    cfgfile = NULL;
	}

	return;
    }

    if (cfgfile != NULL)
	delete cfgfile;

    cfgfile = new ConfigFile (filename);

    if (!cfgfile->isOpenSuccess() ) {
	delete cfgfile;
	cfgfile = NULL;
#ifdef CFGDEBUG
	std::cout << "\topen " << filename << " was not successful" << std::endl;
#endif
    } else {
#ifdef CFGDEBUG
	std::cout << "\topen " << cfgfile->getConfigFilePath() << " successful" << std::endl;

	if (cfgfile->getIgnoreRC() ) {
	    std::cout << "\tRC file says to ignore itself!" << std::endl;
	}

#endif
	cl_ignorerc.ignore = cfgfile->getIgnoreRC();
    }
}

std::string
Config::getPetFile() const {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::getPetFile() const";
    std::cout << ":" << std::endl;
#endif

    if (cl_petfile.set_on_cl) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cmd line: " << cl_petfile.name << std::endl;
#endif
	return cl_petfile.name;
    }

    if (cl_ignorerc.ignore) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from rc file ignored. Taking default: " << Config::def_petfile << std::endl;
#endif
	return Config::def_petfile;
    }

    if (cfgfile != NULL) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cfgfile: " << cfgfile->getFileToLoad() << std::endl;
#endif
	return std::string (cleanupPath (cfgfile->getFileToLoad() ) );
    }

#ifdef CFGDEBUG
    else {
	std::cout << "\tcfgfile == NULL " << std::endl;
    }

#endif
#ifdef CFGDEBUG
    std::cout << "\tsimply returning default value: " << Config::def_petfile << std::endl;
#endif
    return Config::def_petfile;
}

int
Config::getTimeout() const {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::getTimeout() const";
    std::cout << ":" << std::endl;
#endif

    if (cl_timeout.set_on_cl) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cmd line: " << cl_timeout.amount << std::endl;
#endif
	return cl_timeout.amount;
    }

    if (cl_ignorerc.ignore) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from rc file ignored. Taking default: " << Config::def_timeout << std::endl;
#endif
	return Config::def_timeout;
    }

    if (cfgfile != NULL) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cfgfile: " << cfgfile->getLockTimeout() << std::endl;
#endif
	return cfgfile->getLockTimeout();
    }

#ifdef CFGDEBUG
    else {
	std::cout << "\tcfgfile == NULL " << std::endl;
    }

#endif
#ifdef CFGDEBUG
    std::cout << "\tsimply returning default value: " << Config::def_timeout << std::endl;
#endif
    return Config::def_timeout;
}

bool
Config::getFilesecurity() const {
#ifdef CFGDEBUG
    std::cout << " === ";
    std::cout << "Config::getFilesecurity() const";
    std::cout << ":" << std::endl;
#endif

    if (cl_filesecurity.set_on_cl) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cmd line: " << cl_filesecurity.check << std::endl;
#endif
	return cl_filesecurity.check;
    }

    if (cl_ignorerc.ignore) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from rc file ignored. Taking default: " << Config::def_filesecurity << std::endl;
#endif
	return Config::def_filesecurity;
    }

    if (cfgfile != NULL) {
#ifdef CFGDEBUG
	std::cout << "\tvalue from cfgfile: " << cfgfile->getUseFileSecurity() << std::endl;
#endif
	return cfgfile->getUseFileSecurity();
    }

#ifdef CFGDEBUG
    else {
	std::cout << "\tcfgfile == NULL " << std::endl;
    }

#endif
#ifdef CFGDEBUG
    std::cout << "\tsimply returning default value: " << Config::def_filesecurity << std::endl;
#endif
    return Config::def_filesecurity;
}


const Config&
Config::operator= (const Config & c) {
    if (&c == this)
	return *this;

    if (c.cfgfile != NULL &&
	    cfgfile != NULL ) {
	delete cfgfile;
	cfgfile = new ConfigFile (* (c.cfgfile) );
    }

    if (c.cfgfile == NULL &&
	    cfgfile != NULL ) {
	delete cfgfile;
	cfgfile = NULL;
    } else {
	cfgfile = NULL;
    }

    cl_petfile = c.cl_petfile;
    cl_timeout = c.cl_timeout;
    cl_filesecurity = c.cl_filesecurity;
    cl_ignorerc = c.cl_ignorerc;
    return *this;
}
