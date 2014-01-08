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

#include "csvexport.h"
#include <structs.h>
#include <file.h>
#include <crypt.h>

std::string
CSVExport::prepareline(const std::string& l) const {
    //
    // see http://tools.ietf.org/html/rfc4180 for an informal
    // definition of csv
    //
    if (l.empty()) return l;

    std::string workstr(l);

    std::string::size_type pos=0;
    // is there a double quote in the string, if so, add another double quote
    while ( (pos=workstr.find('"',pos)) != std::string::npos ) {
	workstr.insert(pos, 1, '"');
	if ((pos+=2) > (workstr.size()-1))
	    break;
    }

    if (workstr.find(' ') != std::string::npos ||
	workstr.find('\t') != std::string::npos ||
	workstr.find(separator) != std::string::npos) {
	// there is a white space or a separator, thus sourround with double quotes
	workstr.push_back('"');
	workstr.insert(static_cast<std::string::size_type>(0),1,'"');
    }

    return workstr;
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

CSVExport::CSVExport (std::string src, std::string dst, char sep, bool verb):
        srcfile (src),
        dstfile (dst),
        separator (sep),
        __verbose (verb) {
    if (access (srcfile.c_str(), R_OK | F_OK) == -1)
        throw std::runtime_error ("Cannot access " + srcfile);
}

/**
 * Does the export.
 *
 * @param pw the password set on the destination file.
 */
void
CSVExport::doexport (const char* pw) {
    std::ofstream csvfile (dstfile.c_str() );

    if (!csvfile)
        throw std::runtime_error ("Cannot open " + dstfile);

    YAPET::Key key (pw);
    YAPET::Crypt crypt (key);
    YAPET::File yapetfile (srcfile, key, false);
    std::list<YAPET::PartDec> list = yapetfile.read (key);

    std::list<YAPET::PartDec>::iterator it = list.begin();

    while ( it!=list.end() ) {
	const YAPET::BDBuffer& enc_rec = it->getEncRecord();
	YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = crypt.decrypt<YAPET::PasswordRecord> (enc_rec);
	YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

	csvfile << prepareline(std::string((char*) ptr_pw->name)) << separator
		<< prepareline(std::string((char*) ptr_pw->host)) << separator
		<< prepareline(std::string((char*) ptr_pw->username)) << separator
		<< prepareline(std::string((char*) ptr_pw->password)) << separator
		<< prepareline(std::string((char*) ptr_pw->comment)) << std::endl;

	if (__verbose) {
	    std::cout << ".";
	    std::cout.flush();
	}
	it++;
    }

    if (__verbose) std::cout << std::endl;

    csvfile.close();
}