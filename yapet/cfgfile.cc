// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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
#include <iostream>

#include "consts.h"
#include "cfgfile.h"

using namespace YAPET::CONFIG;

std::string
ConfigFile::getHomeDir() const {
    std::string homedir("");

    char* hd = getenv("HOME");

    if (hd != 0) {
        homedir = hd;

        if (homedir[homedir.length() ] != '/')
            homedir.push_back('/');

        return homedir;
    }

    struct passwd* pwd;
    pwd = getpwuid(getuid() );

    if (pwd != 0) {
        homedir = pwd->pw_dir;

        if (homedir[homedir.length() ] != '/')
            homedir.push_back('/');

        return homedir;
    }

    assert(!homedir.empty() );
    return homedir;
}

ConfigFile::ConfigFile(Config& cfg, std::string cfgfile):
    __cfg(cfg),
    filepath(cfgfile.empty() ?
	     getHomeDir() + YAPET::Consts::default_rcfilename :
	     cfgfile) {

    if (access(filepath.c_str(), R_OK | F_OK) == -1) {
	throw std::runtime_error(std::string(_("Cannot open ") + filepath));
    }
}

ConfigFile::ConfigFile(const ConfigFile& cfgfile) :
    __cfg(cfgfile.__cfg),
    filepath(cfgfile.filepath) {}

const ConfigFile&
ConfigFile::operator=(const ConfigFile& cfgfile) {
    if (&cfgfile == this)
        return *this;

    __cfg = cfgfile.__cfg;
    filepath = cfgfile.filepath;

    return *this;
}

void
ConfigFile::parse() {
    try {
        std::ifstream cfgsin(filepath.c_str() );

        if (cfgsin.bad())
            return;

	char line[YAPET::Consts::max_config_line_length];
        while (cfgsin.getline(line, YAPET::Consts::max_config_line_length) ) {
	    if (std::strlen(line)<1) continue;

	    if (line[0]== '#') continue;

            std::string l(line);

	    std::string::size_type pos = l.find("=");

	    if (pos == std::string::npos) {
		std::cerr << "'" <<line << "' " << _("is invalid configuration option (missing '=')") << std::endl;
		continue;
	    }

	    std::string option(l.substr(0,pos));
	    std::string val(l.substr(pos+1));

	    if (val.empty()) {
		std::cerr << "'" << option << "' " << _("has no value") << std::endl;
		continue;
	    }

	    // here we set the value
	    try {
		__cfg[trim(option)].set_str(val);
	    } catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
	    }
	}
        cfgsin.close();
    } catch (...) {
        // lazy me
    }
}
