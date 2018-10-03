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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "cfgfile.h"
#include "consts.h"
#include "intl.h"

using namespace YAPET::CONFIG;

ConfigFile::ConfigFile(Config& cfg, std::string cfgfile)
    : _cfg(cfg),
      filepath(cfgfile.empty()
                   ? getHomeDir() + YAPET::Consts::DEFAULT_RC_FILENAME
                   : cfgfile) {
    if (access(filepath.c_str(), R_OK | F_OK) == -1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open '%s'"), filepath.c_str());
        throw std::runtime_error(msg);
    }
}

ConfigFile::ConfigFile(const ConfigFile& cfgfile)
    : _cfg(cfgfile._cfg), filepath(cfgfile.filepath) {}

const ConfigFile& ConfigFile::operator=(const ConfigFile& cfgfile) {
    if (&cfgfile == this) return *this;

    _cfg = cfgfile._cfg;
    filepath = cfgfile.filepath;

    return *this;
}

void ConfigFile::parse() {
    try {
        std::ifstream cfgsin(filepath.c_str());

        if (cfgsin.bad()) return;

        char line[YAPET::Consts::MAX_CONFIG_LINE_LENGTH];
        while (cfgsin.getline(line, YAPET::Consts::MAX_CONFIG_LINE_LENGTH)) {
            if (std::strlen(line) < 1) continue;

            if (line[0] == '#') continue;

            std::string l(line);

            std::string::size_type pos = l.find("=");

            if (pos == std::string::npos) {
                char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE * 3];
                std::snprintf(msg,
                              YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE * 3,
                              _("'%s' is not a invalid configuration option "
                                "(missing '=')"),
                              line);
                std::cerr << msg << std::endl;
                continue;
            }

            std::string option(l.substr(0, pos));
            std::string val(l.substr(pos + 1));

            if (val.empty()) {
                char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
                std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                              _("'%s' has no value"), option.c_str());
                std::cerr << msg << std::endl;
                continue;
            }

            // here we set the value
            try {
                _cfg[trim(option)].set_str(val);
            } catch (std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        cfgsin.close();
    } catch (...) {
        // lazy me
    }
}
