// -*- c++ -*-
//
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

#ifndef _CFGFILE_H
#define _CFGFILE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

// Used for the YAPET::PWGEN::RNGENGINE type
#include "pwgen/rng.h"

/**
 * @brief All YAPET configuration stuff is contained herein.
 *
 * All YAPET configuration stuff is contained herein.
 */
namespace YAPET {
    namespace CONFIG {

        /**
         * @brief Parses the configuration file.
         *
         * This class parses the per user configuration file.
         */
        class ConfigFile {
            private:
		/**
		 * Values returned by \c readOption.
		 */
		enum ReadResult {
		    OPTION_NOT_FOUND,
		    OPTION_EMPTY,
		    OPTION_FOUND
		};

                std::string filetoload;
                bool usefsecurity;
                unsigned int locktimeout;
		unsigned int pwinputtimeout;
		bool allowlockquit;
                // Yes, the file can say that it should be ignored!
                bool ignorerc;
                std::string cfgfilepath;

                //! Indicates whether or not the file could have been opened
                bool opensuccess;

		// PWGen settings
		bool pwgen_letters;
		bool pwgen_digits;
		bool pwgen_punct;
		bool pwgen_special;
		bool pwgen_other;
		YAPET::PWGEN::RNGENGINE pwgen_rng;
		size_t pwgen_pwlen;

                std::string getHomeDir() const;
		
		/**
		 * Reads single options into the variable specified.
		 *
		 * @param l line read from the config file
		 *
		 * @param needle name of the option including the '=' sign
		 *
		 * @param ref reference where to store the value read.
		 *
		 * @return a value from ReadResult. \c val is only touched if
		 * return value is \c OPTION_FOUND.
		 */
		template<class t> ReadResult readOption(std::string l, const std::string& needle, t& val)  {
		    if (l.find (needle, 0) == 0) {
			l.erase (0, needle.length() );
			if (l.empty())
			    return OPTION_EMPTY;
			
			std::istringstream sstr(l);
			sstr >> val;
			return OPTION_FOUND;
		    }
		    return OPTION_NOT_FOUND;
		}

                void parseFile();

            public:
                ConfigFile (std::string cfgfile = "");
                ConfigFile (const ConfigFile& cfgfile);
                inline ~ConfigFile() {};


                inline const std::string& getConfigFilePath() const {
                    return cfgfilepath;
                }
                inline const std::string& getFileToLoad() const {
                    return filetoload;
                }
                inline bool getUseFileSecurity() const {
                    return usefsecurity;
                }
                inline unsigned int getLockTimeout() const {
                    return locktimeout;
                }
                inline bool getIgnoreRC() const {
                    return ignorerc ;
                }
		inline unsigned int getPwInputTimeout() const {
		    return pwinputtimeout;
		}
		inline bool getAllowLockQuit() const {
		    return allowlockquit;
		}
		inline YAPET::PWGEN::RNGENGINE getPWGenRNG() const {
		    return pwgen_rng;
		}
		inline size_t getPWGenPWLen() const {
		    return pwgen_pwlen;
		}
		inline bool getPWGenLetters() const {
		    return pwgen_letters;
		}
		inline bool getPWGenDigits() const {
		    return pwgen_digits;
		}
		inline bool getPWGenPunct() const {
		    return pwgen_punct;
		}
		inline bool getPWGenSpecial() const {
		    return pwgen_special;
		}
		inline bool getPWGenOther() const {
		    return pwgen_other;
		}
                inline bool isOpenSuccess() const {
                    return opensuccess;
                }

                const ConfigFile& operator= (const ConfigFile& cfgfile);
        };

    }
}

#endif // _CFGFILE_H
