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

#ifndef _FILEERROR_HH
#define _FILEERROR_HH

#include <cstring>

#include "baseerror.hh"

namespace yapet {
constexpr auto NO_SYSTEM_ERROR_SPECIFIED = -1;

class FileError : public YAPETBaseError {
   private:
    int _errorNumber;
    const char* _systemErrorMsg;

   public:
    FileError(const char* msg, int errorNumber = NO_SYSTEM_ERROR_SPECIFIED)
        : YAPETBaseError{msg},
          _errorNumber{errorNumber},
          _systemErrorMsg{nullptr} {
        if (_errorNumber > NO_SYSTEM_ERROR_SPECIFIED) {
            _systemErrorMsg = std::strerror(_errorNumber);
        }
    }

    int errorNumber() const { return _errorNumber; }
    const char* systemErrorMsg() const { return _systemErrorMsg; }
};

class FileInsecureError : public FileError {
   public:
    FileInsecureError(const char* msg)
        : FileError{msg, NO_SYSTEM_ERROR_SPECIFIED} {}
};

class FileFormatError : public FileError {
   public:
    FileFormatError(const char* msg)
        : FileError{msg, NO_SYSTEM_ERROR_SPECIFIED} {}
};
}  // namespace yapet

#endif