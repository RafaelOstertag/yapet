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

#ifndef _CSVSTRINGFIELD_HH
#define _CSVSTRINGFIELD_HH

#include <string>

namespace yapet {

class CSVStringField {
   private:
    static constexpr char DEFAULT_SEPARATOR{','};

    std::string _escapedString;
    std::string _unescapedString;
    std::string _string;
    char _fieldSeparator;

    bool isEscaped();
    bool requireEscaping();
    std::string performEscapeing();
    std::string performUnescapeing();

   public:
    CSVStringField();
    /**
     * Constructor.
     *
     * @param str CSV field string encoded as per
     * .
     */
    CSVStringField(const std::string& str,
                   char fieldSeparator = CSVStringField::DEFAULT_SEPARATOR);
    CSVStringField(const CSVStringField& other);
    CSVStringField(CSVStringField&& other);
    CSVStringField& operator=(const CSVStringField& other);
    CSVStringField& operator=(CSVStringField&& other);

    /**
     * Unescape string.
     *
     * Unescape string assuming it is escaped as per
     * https://www.ietf.org/rfc/rfc4180.txt
     */
    std::string unescape();

    /**
     * Escape string if necessary.
     *
     * Escape string as per https://www.ietf.org/rfc/rfc4180.txt
     */
    std::string escape();
};

}  // namespace yapet

#endif