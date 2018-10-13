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

#ifndef _YAPETERROR_HH
#define _YAPETERROR_HH

#include "baseerror.hh"

namespace yapet {

/**
 * @brief Exception indicating that an operation may be retried
 *
 * This exception indicates that an operation may be retried. The
 * \c File class uses this exception for certain methods.
 *
 * @sa File
 */
class RetryableError : public YAPETBaseError {
   public:
    RetryableError() noexcept : YAPETBaseError{_("Retry")} {}
    RetryableError(const char* msg) noexcept : YAPETBaseError{msg} {}
};

/**
 * @brief Indicates a wrong password.
 *
 * Indicates that a wrong password, thus key, was used for
 * decryption of a file.
 *
 * @sa File
 */
class InvalidPasswordError : public YAPETBaseError {
   public:
    InvalidPasswordError() noexcept : YAPETBaseError{_("Invalid password")} {}
    InvalidPasswordError(const char* msg) noexcept : YAPETBaseError{msg} {}
};
}  // namespace yapet

#endif