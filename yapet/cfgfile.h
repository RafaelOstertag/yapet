// -*- c++ -*-
//
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

#ifndef _CFGFILE_H
#define _CFGFILE_H


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING
#include <string>
#endif

/**
 * @brief Parses the configuration file.
 *
 * This class parses the per user configuration file.
 */
class ConfigFile {
    private:
	static std::string cfgfile;
	static std::string getCfgFile();

	inline ConfigFile(const ConfigFile&) {}
	inline const ConfigFile& operator=(const ConfigFile&) { return *this; }

	std::string filetoload;
	bool usefsecurity;
	unsigned int locktimeout;
	std::string cfgfilepath;


	std::string getHomeDir() const;
	void parseFile();

    public:
	ConfigFile();
	inline ~ConfigFile() {};

	inline std::string getFileToLoad() const { return filetoload; }
	inline bool getUseFileSecurity() const { return usefsecurity; }
	inline unsigned int getLockTimeout() const { return locktimeout; }
};

#endif // _CFGFILE_H
