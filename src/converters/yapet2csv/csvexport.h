// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2014  Rafael Ostertag
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

#ifndef _CSVEXPORT_H
#define _CSVEXPORT_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>
#include <stdexcept>
#include <string>

/**
 * The class taking care of converting a pet file to a csv file.
 */
class CSVExport {
   private:
    /**
     * The file path of the source (csv) file.
     */
    std::string srcfile;
    /**
     * The file path of the destination (pet) file.
     */
    std::string dstfile;
    /**
     * The separator character.
     */
    char separator;
    /**
     * Verbosity enabled/disabled.
     */
    bool _verbose;

    /**
     * Print header
     */
    bool _print_header;

    std::string prepareline(const std::string& l) const;

   public:
    CSVExport(std::string src, std::string dst, char sep, bool verb = true,
              bool print_header = false);
    ~CSVExport(){};
    /// Do the import.
    void doexport(const char* pw);
    /// Set the verbosity.
    inline void verbose(bool v) { _verbose = v; }
    /// Get the verbosity.
    inline bool verbose() const { return _verbose; }
};

#endif  // _CSVEXPORT_H
