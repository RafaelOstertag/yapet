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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

#include "consts.h"
#include "cryptofactoryhelper.hh"
#include "csvexport.h"
#include "csvline.hh"
#include "file.hh"
#include "passwordrecord.hh"

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

CSVExport::CSVExport(std::string src, std::string dst, char sep, bool verb,
                     bool print_header)
    : srcfile(src),
      dstfile(dst),
      separator(sep),
      _verbose(verb),
      _print_header(print_header) {
    if (access(srcfile.c_str(), R_OK | F_OK) == -1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot access '%s'"), srcfile.c_str());
        throw std::runtime_error(msg);
    }
}

namespace {
inline std::ofstream openCsvFile(const std::string& fileName) {
    std::ofstream csvFile(fileName);

    if (!csvFile) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open '%s'"), fileName.c_str());
        throw std::runtime_error(msg);
    }

    return csvFile;
}
}  // namespace

/**
 * Does the export.
 *
 * @param pw the password set on the destination file.
 */
void CSVExport::doexport(const char* pw) {
    auto csvFile{::openCsvFile(dstfile)};

    auto password{yapet::toSecureArray(pw)};
    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
        yapet::getCryptoFactoryForFile(srcfile, password)};

    auto crypto{cryptoFactory->crypto()};
    std::unique_ptr<YAPET::File> yapetFile{
        new YAPET::File{cryptoFactory, srcfile, false, false}};

    std::list<yapet::PasswordListItem> list = yapetFile->read();

    std::list<yapet::PasswordListItem>::iterator it = list.begin();

    yapet::CSVLine csvLine{5, separator};
    if (!list.empty() && _print_header) {
        csvLine.addField(0, std::string{"name"});
        csvLine.addField(1, std::string{"host"});
        csvLine.addField(2, std::string{"username"});
        csvLine.addField(3, std::string{"password"});
        csvLine.addField(4, std::string{"comment"});
        csvFile << csvLine.getLine() << std::endl;
    }

    while (it != list.end()) {
        auto decryptedPasswordRecord{crypto->decrypt(it->encryptedRecord())};
        yapet::PasswordRecord passwordRecord{decryptedPasswordRecord};

        csvLine.addField(
            0,
            std::string(reinterpret_cast<const char*>(passwordRecord.name())));
        csvLine.addField(
            1,
            std::string(reinterpret_cast<const char*>(passwordRecord.host())));
        csvLine.addField(2, std::string(reinterpret_cast<const char*>(
                                passwordRecord.username())));
        csvLine.addField(3, std::string(reinterpret_cast<const char*>(
                                passwordRecord.password())));
        csvLine.addField(4, std::string(reinterpret_cast<const char*>(
                                passwordRecord.comment())));

        csvFile << csvLine.getLine() << std::endl;

        if (_verbose) {
            std::cout << ".";
            std::cout.flush();
        }
        it++;
    }

    if (_verbose) std::cout << std::endl;

    csvFile.close();
}
