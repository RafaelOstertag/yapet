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

#include <cassert>
#include <string>
#include <algorithm>
#include <functional>
#include <limits>
#include <sstream>
#include <map>

#include <sys/types.h>

#include "rng.h"
#include "consts.h"
#include "cfgfile.h"

namespace YAPET {
    namespace CONFIG {
	namespace INTERNAL {
	    // Wrapper function object which returns bool so that it
	    // can be used by *_if algorithms
	    class IntIsTrue {
		    std::pointer_to_unary_function<int,int> __f;
		public:
		    IntIsTrue(std::pointer_to_unary_function<int,int> _f) : __f(_f) {}
		    bool operator()(int i) {
			return std::not_equal_to<int>()(0,__f(i));
		    }
	    };
	}
	    
	class CfgValBase {
public:
		virtual void set_str(const std::string&) = 0;
		virtual ~CfgValBase() {}
	};

        /**
         * @brief template class used for configuration values
         *
         * This template class is mainly used for configuration
         * values. It allows the values being locked, i.e. preventing
         * the values being changed by set methods.
         */
        template<class T> class CfgVal : public CfgValBase {
            private:
                //! Indicates whether or not the value can be changed.
                bool locked;
                T val;

	    protected:
		std::string remove_space(const std::string& str) {
		    std::string space_clean;
		    space_clean.reserve(str.size());
		    
		    INTERNAL::IntIsTrue space(std::ptr_fun<int,int>(std::isspace));
		    std::remove_copy_if(str.begin(), str.end(),
					space_clean.begin(),
					space);

		    return space_clean;
		}

		std::string tolower(const std::string& str) {
		    std::string lower(str);
		    std::transform(lower.begin(), lower.end(), lower.begin(),
				   std::ptr_fun<int, int>(std::tolower));
		    
		    return lower;
		}

            public:
		typedef typename T type;

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

		virtual void set(const T& v) {
                    if (!locked)
                        val = v;
                }
		
		virtual T get() const {
                    return val;
                }

                void lock() {
                    locked = true;
                }

                void unlock() {
                    locked = false;
                }

                void is_locked() const {
                    return locked;
                }

		operator T() const {
		    return val;
		}
        };

	typedef CfgVal<std::string> CfgValStr;

	class CfgValPetFile : public CfgValStr {
	    private:
		std::string cleanup_path(const std::string& p);

	    public:
		CfgValPetFile(std::string v) : CfgValStr(cleanup_path(v)) {}
		CfgValPetFile(const CfgValPetFile& cv): CfgValStr(cv) {}
		CfgValPetFile& operator=(const CfgValPetFile& cv) {
		    CfgValStr::operator=(cv);
		    return *this;
		}
		CfgValPetFile& operator=(const std::string b) {
		    CfgValStr::operator=(b);
		    return *this;
		}
		
		void set(const std::string& s);
		void set_str(const std::string& s);
	};

	class CfgValBool : public CfgVal<bool> {
	    public:
		CfgValBool(bool v) : CfgVal<bool>(v) {}
		CfgValBool(const CfgValBool& cv): CfgVal<bool>(cv) {}
		CfgValBool& operator=(const CfgValBool& cv) {
		    CfgVal<bool>::operator=(cv);
		    return *this;
		}
		CfgValBool& operator=(const bool b) {
		    CfgVal<bool>::operator=(b);
		    return *this;
		}
		
		void set_str(const std::string& s);
	};

	class CfgValInt : public CfgVal<int> {
	    private:
		int __min;
		int __max;
		int __def_out_of_bounds;

	    public:
		CfgValInt(int v,
			  int def_out_of_bounds=0,
			  int min=std::numeric_limits<int>::min(),
			  int max=std::numeric_limits<int>::max()) : CfgVal<int>(v < min || v > max ? def_out_of_bounds : v), __min(min), __max(max), __def_out_of_bounds(def_out_of_bounds) {
		    assert(__def_out_of_bounds >= __min);
		    assert(__def_out_of_bounds <= __max);
}
		CfgValInt(const CfgValInt& cv): CfgVal<int>(cv) {}
		CfgValInt& operator=(const CfgValInt& cv) {
		    CfgVal<int>::operator=(cv);
		    return *this;
		}
		CfgValInt& operator=(const int b) {
		    CfgVal<int>::operator=(b);
		    return *this;
		}

		void set(const int& i) {
		    CfgVal<int>::set(i < __min || i > __max ? __def_out_of_bounds : i);
		}
		
		void set_str(const std::string& s);

		operator std::string() const {
		    std::ostringstream conv;
		    conv << get();
		    return conv.str();
		}
	};

	class CfgValRNG : public CfgVal<YAPET::PWGEN::RNGENGINE> {
	    public:
		CfgValRNG(YAPET::PWGEN::RNGENGINE v) : CfgVal<YAPET::PWGEN::RNGENGINE>(v) {}
		CfgValRNG(const CfgValRNG& cv): CfgVal<YAPET::PWGEN::RNGENGINE>(cv) {}
		CfgValRNG& operator=(const CfgValRNG& cv) {
		    CfgVal<YAPET::PWGEN::RNGENGINE>::operator=(cv);
		    return *this;
		}
		CfgValRNG& operator=(const YAPET::PWGEN::RNGENGINE b) {
		    CfgVal<YAPET::PWGEN::RNGENGINE>::operator=(b);
		    return *this;
		}
		
		void set_str(const std::string& s);
	};

        /**
         * @brief Handle the command line and config file options.
         *
         * This class handles the command line and config file options.
         */
        class Config {
            private:
		// mainly used by ConfigFile;
		std::map<std::string,CfgValBase*> __options;
                //! Removes two or more consecutive slashes from the path
                std::string cleanupPath(const std::string& s) const;

		void setup_map();

            public:
                CfgValPetFile petfile;
                CfgValInt timeout;
                CfgValBool filesecurity;
                CfgValInt pwgenpwlen;
                CfgValRNG pwgen_rng;
                CfgValInt character_pools;
                CfgValBool allow_lock_quit;
                CfgValInt pw_input_timeout;

                Config();
                Config(const Config& c);
                ~Config();

                const Config& operator=(const Config& c);

		//! Lock all configuration values
		void lock();

		//! Unlock all configuration values
		void unlock();

		CfgValBase& operator[](const std::string& key);
        };
    }
}

#endif // _CFG_H
