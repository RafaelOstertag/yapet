// $Id$
//
// Copyright (C) 2009-2010  Rafael Ostertag
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
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <unistd.h>

#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "csvimport.h"
#include <structs.h>
#include <file.h>

/**
 * Removes the double quotes at the beginning and any escaped double quotes.
 *
 * @param str the string to be cleaned up.
 *
 * @return the string \c str passed will be modified.
 */
void
CSVImport::cleanupValue (std::string& str) {
    if (str.length() == 0) return;

    if (str.at (0) == '"')
        str = str.erase (0, 1);

    if (str.at (str.length() - 1) == '"')
        str = str.erase (str.length() - 1, 1);

    std::string::size_type pos = 0;

    while ( ( pos = str.find ("\"\"", pos) ) != std::string::npos ) {
        str.erase (pos, 1);

        if (pos + 1 < str.length() )
            pos++;
    }
}

/**
 * Logs the given error.
 *
 * @param lno the line number the error occurred.
 *
 * @param errmsg the error message.
 */
void
CSVImport::logError (unsigned long lno, const std::string& errmsg) {
    if (verbose) {
        std::cout << 'e';
        std::cout.flush();
    }

    LogEntry tmp;
    tmp.lineno = lno;
    tmp.message = errmsg;
    logs.push_back (tmp);
    had_errors = true;
    num_errors++;
}

/**
 * The constructor tests whether the given source file exists and can be
 * read. May return a \c std::runtime_error if this is not the case
 *
 * @param src the file path of the source file.
 *
 * @param dst the file path of the destination file.
 *
 * @param sep the separator used for fields.
 *
 * @param verb enable/disable verbosity. Default \c true.
 */

CSVImport::CSVImport (std::string src, std::string dst, char sep, bool verb) throw (std::runtime_error) :
        srcfile (src),
        dstfile (dst),
        separator (sep),
        verbose (verb),
        had_errors (false),
        num_errors (0) {
    if (access (srcfile.c_str(), R_OK | F_OK) == -1)
        throw std::runtime_error ("Cannot access " + srcfile);
}

/**
 * Does the import.
 *
 * @param pw the password set on the destination file.
 */
void
CSVImport::import (const char* pw) throw (std::exception) {
    std::ifstream csvfile (srcfile.c_str() );

    if (!csvfile)
        throw std::runtime_error ("Cannot open " + srcfile);

    // the max line length. Computed from the field sizes of a YAPET password
    // record.
    const int max_len = YAPET::NAME_SIZE +
                        YAPET::HOST_SIZE +
                        YAPET::USERNAME_SIZE +
                        YAPET::PASSWORD_SIZE +
                        YAPET::COMMENT_SIZE +
                        // for the separators
                        NUM_SEPARATORS;
    // used for logging purpose
    const unsigned int num_fields = NUM_SEPARATORS + 1;
    char num_fields_str[5];
    snprintf (num_fields_str, 5, "%u", num_fields);
    YAPET::Key key (pw);
    YAPET::File yapetfile (dstfile, key, true);
    std::list<YAPET::PartDec> list;
    char line[max_len];
    // Holds the line number count
    unsigned long lineno = 0;
    // Flag indicating whether we're in a quote or not
    bool inquote = false;
    // Array holding the field values of the csv file
    std::string field_values[num_fields];
    // Integer indicating which field has to be filled
    int current_field = 0;
    // Number of delimiters found per line
    int num_sep_found = 0;
    // The iterator for scanning the line character by character
    std::string::size_type it = 0;

    while (csvfile.getline (line, max_len) ) {
        lineno++;
        std::string l (line);
        // integer pointing to the last delimiter found
        std::string::size_type last_sep = 0;

        if (!inquote) {
            current_field = 0;
            num_sep_found = 0;

            for (unsigned int i = 0; i < num_fields; i++)
                field_values[i].clear();
        }

        // Indicate scanning error
        bool scan_error = false;

        for (it = 0; it < l.length(); it++) {
            // Flip the inquote flag when encountering a double quote
            if (l.at (it) == '"') {
                inquote = !inquote;
            }

            if (!inquote && (l.at (it) == separator) ) {
                num_sep_found++;

                if (num_sep_found > NUM_SEPARATORS) {
                    std::string tmp ("Too many fields. Expected ");
                    tmp += num_fields_str;
                    tmp += " fields.";
                    logError (lineno, tmp );
                    scan_error = true;
                    break;
                }

                field_values[current_field] = l.substr (last_sep, it - last_sep);
                cleanupValue (field_values[current_field]);
                last_sep = it + 1;
                current_field++;
            }
        }

        if (scan_error) continue;

        // Make sure the last field will be extracted too, but check if the
        // last separator has any value followed.
        if (it > last_sep) {
            field_values[current_field] = l.substr (last_sep, it - last_sep);
            cleanupValue (field_values[current_field]);
        }

        if (!inquote && (num_sep_found < NUM_SEPARATORS) ) {
            std::string tmp ("Too few fields. Expected ");
            tmp += num_fields_str;
            tmp += " fields.";
            logError (lineno, tmp );
            continue;
        }

        if (!inquote && (num_sep_found == NUM_SEPARATORS) ) {
            YAPET::Record<YAPET::PasswordRecord> record;
            YAPET::PasswordRecord *ptr_rec = record;
            strncpy ( (char*) ptr_rec->name, field_values[0].c_str(), YAPET::NAME_SIZE);
            strncpy ( (char*) ptr_rec->host, field_values[1].c_str(), YAPET::HOST_SIZE);
            strncpy ( (char*) ptr_rec->username, field_values[2].c_str(), YAPET::USERNAME_SIZE);
            strncpy ( (char*) ptr_rec->password, field_values[3].c_str(), YAPET::PASSWORD_SIZE);
            strncpy ( (char*) ptr_rec->comment, field_values[4].c_str(), YAPET::COMMENT_SIZE);
            list.push_back (YAPET::PartDec (record, key) );

            if (verbose) {
                std::cout << ".";
                std::cout.flush();
            }
        }
    }

    if (verbose) std::cout << std::endl;

    yapetfile.save (list);
    csvfile.close();
}

/**
 * Prints the log entries to stdout.
 */
void
CSVImport::printLog() const {
    if (logs.size() == 0) return;

    std::list<LogEntry>::const_iterator it = logs.begin();

    while (it != logs.end() ) {
        std::cout << "Line " << (*it).lineno << ": " << (*it).message << std::endl;
        it++;
    }
}
