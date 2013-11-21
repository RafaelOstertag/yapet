// -*- c++ -*-
//
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

#ifndef _CFG_H
#define _CFG_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>

#include <sys/types.h>

#include "pwgen/rng.h"
#include "consts.h"
#include "cfgfile.h"

namespace YAPET {
    namespace CONFIG {
        /**
         * @brief template class used for configuration values
         *
         * This template class is mainly used for configuration
         * values. It allows the values being locked, i.e. preventing
         * the values being changed by set methods.
         */
        template<class T> class CfgVal {
            private:
                //! Indicates whether or not the value can be changed.
                bool locked;
                T val;

            public:
                CfgVal() : locked(false) {
                }

                CfgVal(const T& v) : val(v) {
                }

                CfgVal(const CfgVal& c) {
                    locked = c.locked;
                    val = c.val;
                }

                CfgVal& operator=(const CfgVal& c) {
                    if (&c == this)
                        return *this;

                    locked = c.locked;
                    val = c.val;

                    return *this;
                }

                CfgVal& operator=(const T& c) {
                    if (!locked)
                        val = c;

                    return *this;
                }

                void set(const T& v) {
                    if (!locked)
                        val = v;
                }

                T get() const {
                    return val;
                }

                void lock() {
                    locked = true;
                }

                void unlock() {
                    locked = false;
                }

                void isLocked() const {
                    return locked;
                }

		operator T() const {
		    return val;
		}
        };

        /**
         * @brief Handle the command line and config file options.
         *
         * This class handles the command line and config file options.
         */
        class Config {
            private:
                //! The default .pet file to open
                static const std::string def_petfile;
                //! The default lock timeout
                static const int def_timeout;
                //! Default for checking file security
                static const bool def_filesecurity;
                //! Default for ignoring the rc file
                static const bool def_ignorerc;
                //! The default password length
                static const size_t def_pwlen;
                //! Default rng
                static const YAPET::PWGEN::RNGENGINE def_pwgen_rng;
                //! The default character subpools to use
                static const int def_character_pools;
                //! Show the Quit button when unlocking screen
                static const bool def_allow_lock_quit;
                //! Default for password input timeout
                static const unsigned int def_pw_input_timeout;

                //! Removes two or more consecutive slashes from the path
                std::string cleanupPath(const std::string& s) const;

            public:
                static std::string getDefPetfile();

                static unsigned int getDefTimeout();

                static bool getDefFilesecurity();

                static bool getDefIgnorerc();

                static YAPET::PWGEN::RNGENGINE getDefPWGenRNG();

                static size_t getDefPWLength();

                static int getDefCharPools();

                static bool getDefAllowLockQuit();

                static unsigned int getDefPwInputTimeout();

                // Those are for convenience
                static bool getDefCPoolLetters();

                static bool getDefCPoolDigits();

                static bool getDefCPoolPunct();

                static bool getDefCPoolSpecial();

                static bool getDefCPoolOther();

                Config();
                Config(const Config& c);
                ~Config();

                void loadConfigFile(std::string filename="");

                CfgVal<std::string> petfile;
                CfgVal<int> timeout;
                CfgVal<bool> filesecurity;
                CfgVal<bool> ignorerc;
                CfgVal<size_t> pwgenpwlen;
                CfgVal<YAPET::PWGEN::RNGENGINE> pwgen_rng;
                CfgVal<int> character_pools;
                CfgVal<bool> allow_lock_quit;
                CfgVal<int> pw_input_timeout;

                /**
                 * @brief Set the file to open upon start of YAPET.
                 *
                 * Set the file to open upon start of YAPET. It will
                 * make sure that the proper suffix is appended.
                 *
                 * @param s the file path of the file.
                 */
                void setPetFile(std::string s) {
                    if (s.find(YAPET::CONSTS::Consts::getDefaultSuffix(),
                               s.length() -
                               YAPET::CONSTS::Consts::getDefaultSuffix().
                               length() )
                        == std::string::npos)
                        s += YAPET::CONSTS::Consts::getDefaultSuffix();

                    s = cleanupPath(s);
                    petfile = s;
                }

                /**
                 * @brief lock all values
                 *
                 * A convenient method for locking all values.
                 */
                void lockAll() {
                    petfile.lock();
                    timeout.lock();
                    filesecurity.lock();
                    ignorerc.lock();
                    pwgenpwlen.lock();
                    pwgen_rng.lock();
                    character_pools.lock();
                    allow_lock_quit.lock();
                    pw_input_timeout.lock();
                }

                /**
                 * @brief unlock all values
                 *
                 * A convenient method for unlocking all values.
                 */
                void unlockAll() {
                    petfile.unlock();
                    timeout.unlock();
                    filesecurity.unlock();
                    ignorerc.unlock();
                    pwgenpwlen.unlock();
                    pwgen_rng.unlock();
                    character_pools.unlock();
                    allow_lock_quit.unlock();
                    pw_input_timeout.unlock();
                }

                const Config& operator=(const Config& c);
        };
    }
}

#endif // _CFG_H
