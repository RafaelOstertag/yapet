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

#ifndef _CSVIMPORT_H
#define _CSVIMPORT_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>
#include <memory>
#include <string>

#include "crypto.hh"
#include "csvline.hh"
#include "passwordlistitem.hh"

/**
 * The class taking care of converting a csv file.
 */
class CSVImport {
   public:
    using line_number_type = unsigned int;
    /**
     * Log entry.
     */
    struct LogEntry {
        line_number_type lineNumber;
        std::string message;
    };

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
    bool verbose;
    /**
     * Indicates the occurrence of errors.
     */
    bool had_errors;
    /**
     * The number of errors occurred.
     */
    unsigned int num_errors;

    /**
     * Holds the log entries in case of errors.
     */
    std::list<LogEntry> logs;

    /**
     * Logs the given error.
     *
     * @param lno the line number the error occurred.
     *
     * @param errmsg the error message.
     */
    void logError(unsigned long lno, const std::string& errmsg);

    yapet::PasswordListItem csvLineToPasswordRecord(
        yapet::CSVLine& csvLine, std::unique_ptr<yapet::Crypto>& crypto);

   public:
    CSVImport(std::string src, std::string dst, char sep, bool verb = true);
    CSVImport(const CSVImport&) = delete;
    CSVImport(CSVImport&&) = delete;
    CSVImport& operator=(const CSVImport&) = delete;
    CSVImport& operator=(CSVImport&&) = delete;
    ~CSVImport(){};

    //! Do the import.
    void import(const char* pw);
    //! Set the verbosity.
    inline void setVerbose(bool v) { verbose = v; }
    //! Get the verbosity.
    inline bool getVerbose() const { return verbose; }
    //! Return whether or not errors occurred.
    inline bool hadErrors() const { return had_errors; }
    //! Return the number of errors occurred.
    inline unsigned int numErrors() const { return num_errors; }
    //! Return the log entries.
    inline std::list<LogEntry> getLog() const { return logs; }
    //! Print the log to stdout
    void printLog() const;
};

#endif  // _CSVIMPORT_H
