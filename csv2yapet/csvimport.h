// -*- c++ -*-
//
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

#ifndef _CSVIMPORT_H
#define _CSVIMPORT_H


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#ifdef HAVE_VECTOR
# include <vector>
#endif

#ifdef HAVE_LIST
# include <list>
#endif

/**
 * Log entry.
 */
struct LogEntry {
	/**
	 * The line number where the error occurred.
	 */
	unsigned int lineno;
	/**
	 * The error message.
	 */
	std::string message;
};

/**
 * The class taking care of importing a csv file.
 */
class CSVImport {
    private:
	/**
	 * Object assignment not allowed.
	 */
	inline const CSVImport& operator=(const CSVImport&) { return *this; }
	/**
	 * Object copying not allowed.
	 */
	inline CSVImport(const CSVImport&) {}

	enum {
	    /**
	     * The expected number of separators.
	     */
	    NUM_SEPARATORS = 4
	};

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

	//! Count the separators of the line provided.
	unsigned int countSeparator(const std::string& s) const;
	//! Returns the position of the separators in the line.
	void getSeparatorPos(const std::string& line,
			     std::vector<std::string::size_type>& posvec) const;
	//! Log the given error
	void logError(unsigned long lno, const std::string& errmsg);

    public:
	CSVImport(std::string src, std::string dst, char sep, bool verb=true) throw(std::runtime_error);
	~CSVImport() {};
	//! Do the import.
	void import(const char* pw) throw(std::exception);
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

#endif // _CSVIMPORT_H
