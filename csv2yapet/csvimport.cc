// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
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
# include <config.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif

#ifdef HAVE_FSTREAM
# include <fstream>
#endif

#include "csvimport.h"
#include <structs.h>
#include <file.h>

/**
 * Count the number of separators on the line.
 *
 * @param s the line
 *
 * @return the number of separators.
 */
unsigned int
CSVImport::countSeparator(const std::string& s) const {
    unsigned int c = 0;
    std::string::size_type pos = 0;
    while ( (pos = s.find(separator, pos)) != std::string::npos ) {
	c++;
	pos++;
    }
    return c;
}

/**
 * Returns a vector holding the character positions of the separators.
 *
 * @param line the line
 *
 * @param posvec the vector filled with the positions
 */
void
CSVImport::getSeparatorPos(const std::string& line,
			   std::vector<std::string::size_type>& posvec) const {
    posvec.clear();
    std::string::size_type pos = 0;
    while ( (pos = line.find(separator, pos)) != std::string::npos ) {
	posvec.push_back(pos);
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
CSVImport::logError(unsigned long lno, const std::string& errmsg) {
    if (verbose) std::cout << 'e';
    LogEntry tmp;
    tmp.lineno = lno;
    tmp.message = errmsg;
    logs.push_back(tmp);
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

CSVImport::CSVImport(std::string src, std::string dst, char sep, bool verb) throw(std::runtime_error) :
    srcfile(src),
    dstfile(dst),
    separator(sep),
    verbose(verb),
    had_errors(false),
    num_errors(0)
{

    if (access(srcfile.c_str(), R_OK | F_OK) == -1)
	throw std::runtime_error("Cannot access " + srcfile);
}

/**
 * Does the import.
 *
 * @param pw the password set on the destination file.
 */
void
CSVImport::import(const char* pw) throw(std::exception) {
    std::ifstream csvfile(srcfile.c_str());

    if (!csvfile)
	throw std::runtime_error("Cannot open " + srcfile);

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
    unsigned int num_fields = NUM_SEPARATORS + 1;
    char num_fields_str[5];
    snprintf(num_fields_str, 5, "%u", num_fields);

    YAPET::Key key(pw);
    YAPET::File yapetfile(dstfile, key, true);
    std::list<YAPET::PartDec> list;

    char line[max_len];
    std::vector<std::string::size_type> seppos;
    unsigned long lineno = 0;
    while (csvfile.getline(line, max_len) ) {
	lineno++;
	std::string l(line);
	if (countSeparator(l) > NUM_SEPARATORS) {
	    std::string tmp("Too many fields. Expected ");
	    tmp += num_fields_str;
	    tmp += " fields.";
	    logError(lineno, tmp );
	    continue;
	}
	if (countSeparator(l) < NUM_SEPARATORS) {
	    std::string tmp("Too few fields. Expected ");
	    tmp += num_fields_str;
	    tmp += " fields.";
	    logError(lineno, tmp );
	    continue;
	}

	getSeparatorPos(l, seppos);
	if (seppos.size() == 0) {
	    logError(lineno, "Unable to identify separators.");
	    continue;
	}

	std::string name = l.substr(0, seppos[0]);
	std::string host = l.substr(seppos[0]+1, seppos[1] - seppos[0] - 1);
	std::string uname = l.substr(seppos[1]+1, seppos[2] - seppos[1] - 1);
	std::string passwd = l.substr(seppos[2]+1, seppos[3] - seppos[2] - 1 );
	std::string comment = l.substr(seppos[3]+1, l.length() - seppos[3]);
	YAPET::Record<YAPET::PasswordRecord> record;
	YAPET::PasswordRecord *ptr_rec = record;
	strncpy((char*)ptr_rec->name, name.c_str(), YAPET::NAME_SIZE);
	strncpy((char*)ptr_rec->host, host.c_str(), YAPET::HOST_SIZE);
	strncpy((char*)ptr_rec->username, uname.c_str(), YAPET::USERNAME_SIZE);
	strncpy((char*)ptr_rec->password, passwd.c_str(), YAPET::PASSWORD_SIZE);
	strncpy((char*)ptr_rec->comment, comment.c_str(), YAPET::COMMENT_SIZE);
	list.push_back(YAPET::PartDec(record, key));
	if (verbose) std::cout << ".";
    }
    if (verbose) std::cout << std::endl;
    yapetfile.save(list);
    csvfile.close();
}

/**
 * Prints the log entries to stdout.
 */
void
CSVImport::printLog() const {
    if (logs.size() == 0) return;

    std::list<LogEntry>::const_iterator it = logs.begin();

    while (it != logs.end()) {
	std::cout << "Line " << (*it).lineno << ": " << (*it).message << std::endl;
	it++;
    }
}
