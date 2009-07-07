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

struct LogEntry {
	unsigned int lineno;
	std::string message;
};

class CSVImport {
    private:
	inline const CSVImport& operator=(const CSVImport&) { return *this; }
	inline CSVImport(const CSVImport&) {}

	enum {
	    NUM_SEPARATORS = 4
	};

	std::string srcfile;
	std::string dstfile;
	char separator;

	bool verbose;
	bool had_errors;
	unsigned int num_errors;

	std::list<LogEntry> logs;

	unsigned int countSeparator(const std::string& s) const;
	void getSeparatorPos(const std::string& line,
			     std::vector<std::string::size_type>& posvec) const;

	void logError(unsigned long lno, const std::string& errmsg);

    public:
	CSVImport(std::string src, std::string dst, char sep, bool verb=true) throw(std::runtime_error);
	~CSVImport() {};
	void import(const char* pw) throw(std::exception);
	inline void setVerbose(bool v) { verbose = v; }
	inline bool getVerbose() const { return verbose; }
	inline bool hadErrors() const { return had_errors; }
	inline unsigned int numErrors() const { return num_errors; }
	inline std::list<LogEntry> getLog() const { return logs; }
	void printLog() const;
};

#endif // _CSVIMPORT_H
