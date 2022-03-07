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

#include "aes256factory.hh"
#include "consts.h"
#include "csvimport.h"
#include "csvline.hh"
#include "file.hh"
#include "intl.h"

constexpr char NEW_LINE_CHARACTER{'\n'};
constexpr char DOUBLE_QUOTE{'"'};

constexpr auto NUMBER_OF_FIELDS{5};
// the max line length. Computed from the field sizes of a YAPET password
// record.
constexpr int MAX_LINE_LENGTH{
    yapet::PasswordRecord::NAME_SIZE + yapet::PasswordRecord::HOST_SIZE +
    yapet::PasswordRecord::USERNAME_SIZE +
    yapet::PasswordRecord::PASSWORD_SIZE + yapet::PasswordRecord::COMMENT_SIZE +
    // for the separators
    NUMBER_OF_FIELDS -
    1
    // null terminators, one for each field
    - 5};

void CSVImport::logError(unsigned long lno, const std::string& errmsg) {
    if (verbose) {
        std::cout << 'e';
        std::cout.flush();
    }

    LogEntry tmp;
    tmp.lineNumber = lno;
    tmp.message = errmsg;

    logs.push_back(tmp);
    had_errors = true;
    num_errors++;
}

yapet::PasswordListItem CSVImport::csvLineToPasswordRecord(
    yapet::CSVLine& csvLine, std::unique_ptr<yapet::Crypto>& crypto) {
    yapet::PasswordRecord passwordRecord;

    passwordRecord.name(csvLine[0].c_str());
    passwordRecord.host(csvLine[1].c_str());
    passwordRecord.username(csvLine[2].c_str());
    passwordRecord.password(csvLine[3].c_str());
    passwordRecord.comment(csvLine[4].c_str());

    auto serializedRecord{passwordRecord.serialize()};
    auto encryptedRecord{crypto->encrypt(serializedRecord)};

    return yapet::PasswordListItem{csvLine[0].c_str(), encryptedRecord};
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

CSVImport::CSVImport(std::string src, std::string dst, char sep, bool verb)
    : srcfile(src),
      dstfile(dst),
      separator(sep),
      verbose(verb),
      had_errors(false),
      num_errors(0) {
    if (access(srcfile.c_str(), R_OK | F_OK) == -1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot access '%s'"), srcfile.c_str());
        throw std::runtime_error(msg);
    }
}

namespace {
inline std::ifstream openCsvFile(const std::string& fileName) {
    std::ifstream file{fileName};

    if (!file) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open '%s'"), fileName.c_str());
        throw std::runtime_error(msg);
    }

    return file;
}

inline char read(std::ifstream& csvFile, char& lookAhead) {
    char tmp = lookAhead;
    csvFile.get(lookAhead);
    return tmp;
}

/**
 * Read a line and honor escaped newline character.
 */
inline std::string readLine(std::ifstream& csvFile, char separator,
                            CSVImport::line_number_type& lineNumber) {
    std::string line;

    bool inEscapedField = false;
    bool startOfLine = true;

    char currentCharacter;
    char lookAhead;

    csvFile.get(lookAhead);
    while (!csvFile.eof()) {
        currentCharacter = read(csvFile, lookAhead);

        if (currentCharacter == NEW_LINE_CHARACTER) {
            lineNumber++;
        }

        if (currentCharacter == NEW_LINE_CHARACTER && !inEscapedField) {
            csvFile.putback(lookAhead);
            csvFile.clear();
            break;
        }

        if (startOfLine && currentCharacter == DOUBLE_QUOTE) {
            startOfLine = false;
            inEscapedField = true;
            line += DOUBLE_QUOTE;
            continue;
        }

        if (currentCharacter == DOUBLE_QUOTE && lookAhead == DOUBLE_QUOTE &&
            inEscapedField) {
            line += currentCharacter;
            line += read(csvFile, lookAhead);
            continue;
        }

        if (currentCharacter == DOUBLE_QUOTE &&
            (lookAhead == separator || lookAhead == NEW_LINE_CHARACTER)) {
            inEscapedField = false;
            line += currentCharacter;
            continue;
        }

        if (currentCharacter == separator && !inEscapedField) {
            line += currentCharacter;

            if (lookAhead == DOUBLE_QUOTE) {
                inEscapedField = true;
                line += read(csvFile, lookAhead);
            }
            continue;
        }

        line += currentCharacter;
        startOfLine = false;
    }

    if (inEscapedField) {
        throw std::invalid_argument(_("'\"' mismatch"));
    }

    return line;
}

}  // namespace

/**
 * Does the import.
 *
 * @param pw the password set on the destination file.
 */
void CSVImport::import(const char* pw) {
    auto csvFile{::openCsvFile(srcfile)};

    auto password{yapet::toSecureArray(pw)};
    auto keyingParameters{yapet::Key256::newDefaultKeyingParameters()};
    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
        new yapet::Aes256Factory{password, keyingParameters}};
    auto crypto{cryptoFactory->crypto()};
    std::unique_ptr<YAPET::File> yapetFile{
        new YAPET::File{cryptoFactory, dstfile, true}};

    std::list<yapet::PasswordListItem> list;

    line_number_type lineNumber = 0;

    yapet::CSVLine csvLine{NUMBER_OF_FIELDS, separator};

    for (std::string line;
         line = ::readLine(csvFile, separator, lineNumber), !csvFile.eof();) {
        if (line.size() > MAX_LINE_LENGTH) {
            logError(lineNumber, _("Line too long"));
            continue;
        }

        try {
            csvLine.parseLine(line);
        } catch (std::exception& e) {
            logError(lineNumber, e.what());
            continue;
        }

        auto passwordListItem{csvLineToPasswordRecord(csvLine, crypto)};
        list.push_back(passwordListItem);

        if (verbose) {
            std::cout << ".";
            std::cout.flush();
        }
    }

    if (verbose) std::cout << std::endl;

    yapetFile->save(list);
    csvFile.close();
}

/**
 * Prints the log entries to stdout.
 */
void CSVImport::printLog() const {
    if (logs.size() == 0) return;

    std::list<LogEntry>::const_iterator it = logs.begin();

    while (it != logs.end()) {
        std::cout << _("Line ") << (*it).lineNumber << ": " << (*it).message
                  << std::endl;
        it++;
    }
}
