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
 * @brief All YAPET configuration stuff is contained herein.
 *
 * All YAPET configuration stuff is contained herein.
 */
namespace YAPETCONFIG {

    /**
     * @brief Parses the configuration file.
     *
     * This class parses the per user configuration file.
     */
    class ConfigFile {
	private:
	    std::string filetoload;
	    bool usefsecurity;
	    unsigned int locktimeout;
	    // Yes, the file can say that it should be ignored!
	    bool ignorerc;
	    std::string cfgfilepath;

	    //! Indicates whether or not the file could have been opened
	    bool opensuccess;

	    std::string getHomeDir() const;
	    void parseFile();

	public:
	    ConfigFile(std::string cfgfile="");
	    ConfigFile(const ConfigFile& cfgfile);
	    inline ~ConfigFile() {};

	    inline const std::string& getFileToLoad() const { return filetoload; }
	    inline bool getUseFileSecurity() const { return usefsecurity; }
	    inline unsigned int getLockTimeout() const { return locktimeout; }
	    inline bool getIgnoreRC() const { return ignorerc ; }
	    inline bool isOpenSuccess() const { return opensuccess; }

	    const ConfigFile& operator=(const ConfigFile& cfgfile);
    };

}

#endif // _CFGFILE_H
