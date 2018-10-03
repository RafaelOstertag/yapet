// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2011, 2013  Rafael Ostertag
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
#define _CFGFILE_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#include <string>

// Used for the YAPET::PWGEN::RNGENGINE type
#include "rng.hh"

#include "cfg.h"

/**
 * @brief All YAPET configuration stuff is contained herein.
 *
 * All YAPET configuration stuff is contained herein.
 */
namespace YAPET {
namespace CONFIG {
// Forward declaration
class Config;
class CfgValBase;

/**
 * @brief Parses the configuration file.
 *
 * This class parses the per user configuration file.
 */
class ConfigFile {
   private:
    Config& _cfg;
    std::string filepath;

   public:
    ConfigFile(Config& cfg, std::string cfgfile = std::string());
    ConfigFile(const ConfigFile& cfgfile);
    ~ConfigFile(){};

    const ConfigFile& operator=(const ConfigFile& cfgfile);

    void parse();
};
}  // namespace CONFIG
}  // namespace YAPET

#endif  // _CFGFILE_H
