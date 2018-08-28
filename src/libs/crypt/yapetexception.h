// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#ifndef _YAPETEXCEPTION_H
#define _YAPETEXCEPTION_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <exception>
#include <string>

#include "intl.h"

namespace YAPET {

/**
 * New in version 0.6
 *
 * Since version 0.6 we provide an optional error
 * specification. It is entirely optional to use it, but at the
 * time of this writing, the YAPET::File class uses it to
 * determine whether or not retrying to decrypt the using a
 * different structure (FileHeader_32 or FileHeader_64)
 */
enum EXNUM {
    BDBUFFER_TOO_SMALL = 1,
    BDBUFFER_TOO_BIG = 2,
    ZEROPOINTER = 3,
    NOTUSED = 255
};

/**
 * @brief The exception class used for cryptographic classes.
 *
 * Modified in version 0.6.
 *
 * The exception base class used for cryptographic classes.
 */
class YAPETException : public std::exception {
   private:
    EXNUM exnum;
    std::string message;

   protected:
    void setExNum(EXNUM n) { exnum = n; }

   public:
    /**
     * @brief Initializes a default exception message
     *
     * Initializes a default exception message
     */
    YAPETException() noexcept
        : exception(),
          exnum(NOTUSED),
          message(_("Generic exception message")) {}
    /**
     * @brief Initializes with a user specified message
     *
     * Initializes with a user specified message
     *
     * @param msg the message of the exception
     */
    YAPETException(std::string msg) noexcept
        : exception(), exnum(NOTUSED), message(msg) {}
    YAPETException(std::string msg, EXNUM n) noexcept
        : exception(), exnum(n), message(msg) {}
    YAPETException(const YAPETException& ex) noexcept
        : exnum{ex.exnum}, message{ex.message} {}
    YAPETException(YAPETException&& ex) noexcept
        : exnum{ex.exnum}, message{std::move(ex.message)} {}
    virtual ~YAPETException() noexcept { /* empty */
    }

    YAPETException& operator=(const YAPETException& ex) noexcept {
        if (this == &ex) return *this;

        exnum = ex.exnum;
        message = ex.message;
        return *this;
    }

    YAPETException& operator=(YAPETException&& ex) noexcept {
        if (this == &ex) return *this;

        exnum = ex.exnum;
        message = std::move(ex.message);

        return *this;
    }

    virtual const char* what() const noexcept { return message.c_str(); }
    EXNUM getExNum() const noexcept { return exnum; }
};

/**
 * @brief Exception indicating that an operation may be retried
 *
 * This exception indicates that an operation may be retried. The
 * \c File class uses this exception for certain methods.
 *
 * @sa File
 */
class YAPETRetryException : public YAPETException {
   public:
    YAPETRetryException() noexcept : YAPETException{_("Retry")} {}
    YAPETRetryException(std::string msg) noexcept : YAPETException{msg} {}
    YAPETRetryException(const YAPETRetryException& ex) noexcept
        : YAPETException{ex} {}
    YAPETRetryException(YAPETRetryException&& ex) noexcept
        : YAPETException{std::move(ex)} {}
    virtual ~YAPETRetryException() noexcept { /* Empty */
    }

    YAPETRetryException& operator=(const YAPETRetryException& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }

    YAPETRetryException& operator=(YAPETRetryException&& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }
};

/**
 * @brief Indicates an error while encrypting/decrypting.
 *
 * This function indicates an error while encrypting/decrypting
 * data.
 */
class YAPETEncryptionException : public YAPETException {
   public:
    YAPETEncryptionException() noexcept
        : YAPETException{_("Encryption error")} {}
    YAPETEncryptionException(std::string msg) noexcept : YAPETException{msg} {}
    YAPETEncryptionException(const YAPETEncryptionException& ex) noexcept
        : YAPETException{ex} {}
    YAPETEncryptionException(YAPETEncryptionException&& ex) noexcept
        : YAPETException {
        std::move(ex)
    }
    virtual ~YAPETEncryptionException() noexcept { /* Empty */
    }

    YAPETEncryptionException& operator=(
        const YAPETEncryptionException& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }

    YAPETEncryptionException& operator=(
        YAPETEncryptionException&& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }
};

/**
 * @brief Indicates a wrong password.
 *
 * Indicates that a wrong password, thus key, was used for
 * decryption of a file.
 *
 * @sa File
 */
class YAPETInvalidPasswordException : public YAPETException {
   public:
    YAPETInvalidPasswordException() noexcept
        : YAPETException{_("Invalid password")} {}
    YAPETInvalidPasswordException(std::string msg) noexcept
        : YAPETException{msg} {}

    YAPETInvalidPasswordException(
        const YAPETInvalidPasswordException& ex) noexcept
        : YAPETException{ex} {}
    YAPETInvalidPasswordException(YAPETInvalidPasswordException&& ex) noexcept
        : YAPETException{std::move(ex)} {}
    virtual ~YAPETInvalidPasswordException() noexcept { /* Empty */
    }

    YAPETInvalidPasswordException& operator=(
        const YAPETInvalidPasswordException& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }

    YAPETInvalidPasswordException& operator=(
        YAPETInvalidPasswordException&& ex) noexcept {
        if (this == &ex) return *this;

        YAPETException::operator=(ex);
        return *this;
    }
};

}  // namespace YAPET

#endif  // _YAPETEXCEPTION_H
