/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifndef _CFG_H
#define _CFG_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <map>
#include <sstream>
#include <string>

#include <sys/types.h>

#include "cfgfile.h"
#include "consts.h"
#include "rng.hh"

namespace YAPET {
namespace CONFIG {
extern std::string trim(const std::string& s);
extern std::string getHomeDir();

namespace INTERNAL {
// Wrapper function object which returns bool so that it
// can be used by *_if algorithms
class IntIsTrue {
    std::pointer_to_unary_function<int, int> __f;

   public:
    IntIsTrue(std::pointer_to_unary_function<int, int> _f) : __f(_f) {}
    bool operator()(int i) { return std::not_equal_to<int>()(0, __f(i)); }
};

// Wrapper function object which returns bool so that it
// can be used by *_if algorithms
class IntIsFalse {
    std::pointer_to_unary_function<int, int> __f;

   public:
    IntIsFalse(std::pointer_to_unary_function<int, int> _f) : __f(_f) {}
    bool operator()(int i) { return std::equal_to<int>()(0, __f(i)); }
};
}  // namespace INTERNAL

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
template <class T>
class CfgVal : public CfgValBase {
   private:
    //! Indicates whether or not the value can be changed.
    bool locked;
    T val;

   protected:
    std::string remove_space(const std::string& str) {
        std::string space_clean(str);

        INTERNAL::IntIsTrue space(std::ptr_fun<int, int>(std::isspace));

        std::string::iterator new_end =
            std::remove_if(space_clean.begin(), space_clean.end(), space);

        if (new_end == space_clean.end()) return space_clean;

        space_clean.erase(new_end, space_clean.end());

        return space_clean;
    }

    std::string tolower(const std::string& str) {
        std::string lower(str);
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       std::ptr_fun<int, int>(std::tolower));

        return lower;
    }

   public:
    typedef T type;

    CfgVal() : locked(false) {}

    CfgVal(const T& v) : locked(false), val(v) {}

    CfgVal(const CfgVal& c) : locked(c.locked), val(c.val) {}

    CfgVal& operator=(const CfgVal& c) {
        if (&c == this) return *this;

        locked = c.locked;
        val = c.val;

        return *this;
    }

    CfgVal& operator=(const T& c) {
        if (!locked) val = c;

        return *this;
    }

    virtual void set(const T& v) {
        if (!locked) val = v;
    }

    virtual T get() const { return val; }

    void lock() { locked = true; }

    void unlock() { locked = false; }

    bool is_locked() const { return locked; }

    operator T() const { return val; }
};

typedef CfgVal<std::string> CfgValStr;

class CfgValPetFile : public CfgValStr {
   private:
    std::string cleanup_path(const std::string& p);
    std::string add_suffix(const std::string& p);

   public:
    CfgValPetFile(std::string v = std::string())
        : CfgValStr(add_suffix(cleanup_path(v))) {}
    CfgValPetFile(const CfgValPetFile& cv) : CfgValStr(cv) {}
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
    CfgValBool(bool v = true) : CfgVal<bool>(v) {}
    CfgValBool(const CfgValBool& cv) : CfgVal<bool>(cv) {}
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
    CfgValInt(int v = 0, int def_out_of_bounds = 0,
              int min = std::numeric_limits<int>::min(),
              int max = std::numeric_limits<int>::max())
        : CfgVal<int>(v < min || v > max ? def_out_of_bounds : v),
          __min(min),
          __max(max),
          __def_out_of_bounds(def_out_of_bounds) {
        assert(__def_out_of_bounds >= __min);
        assert(__def_out_of_bounds <= __max);
    }
    CfgValInt(const CfgValInt& cv) : CfgVal<int>(cv) {}
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

class CfgValColor : public CfgValStr {
   public:
    void set_str(const std::string& s) { set(s); }
};

/**
 * @brief Handle the command line and config file options.
 *
 * This class handles the command line and config file options.
 */
class Config {
   private:
    // mainly used by ConfigFile;
    std::map<std::string, CfgValBase*> _options;
    //! Removes two or more consecutive slashes from the path
    std::string cleanupPath(const std::string& s) const;

    void setup_map();

   public:
    CfgValPetFile petfile;
    /// @todo rename to locktimeout
    CfgValInt timeout;
    CfgValBool filesecurity;
    CfgValInt pwgenpwlen;
    CfgValBool pwgen_letters;
    CfgValBool pwgen_digits;
    CfgValBool pwgen_punct;
    CfgValBool pwgen_special;
    CfgValBool pwgen_other;
    CfgValBool allow_lock_quit;
    CfgValInt pw_input_timeout;
    // in kibi
    CfgValInt argon2_memory;
    CfgValInt argon2_parallelism;
    CfgValInt argon2_iterations;
    CfgValBool ignorerc;
    CfgValColor colors;

    Config();
    Config(const Config& c);
    Config(Config&& c) = delete;
    ~Config();

    Config& operator=(Config&& c) = delete;
    Config& operator=(const Config& c);

    /**
     * Convenience method.
     *
     * @return @c int representing all selected character
     * pools.
     */
    int character_pools() const;

    //! Lock all configuration values
    void lock();

    //! Unlock all configuration values
    void unlock();

    CfgValBase& operator[](const std::string& key);
};
}  // namespace CONFIG
}  // namespace YAPET

#endif  // _CFG_H
