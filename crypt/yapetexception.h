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
#define _YAPETEXCEPTION_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <exception>
#include <string>

#include "../intl.h"

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
	NULLPOINTER = 3,
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
	    inline void setExNum(EXNUM n) { exnum = n ; }
        public:
            /**
             * @brief Initializes a default exception message
             *
             * Initializes a default exception message
             */
            inline YAPETException()
            throw() : exception(),
		      exnum(NOTUSED),
                    message (_ ("Generic exception message") ) {}
            /**
             * @brief Initializes with a user specified message
             *
             * Initializes with a user specified message
             *
             * @param msg the message of the exception
             */
            inline YAPETException (std::string msg) throw() : exception(),
							      exnum(NOTUSED),
							      message (msg) {}
            inline YAPETException (std::string msg, EXNUM n) throw() : exception(),
								       exnum(n),
								       message (msg) {}
            inline YAPETException (const YAPETException& ex) throw() {
		exnum = ex.exnum;
                message = ex.message;
            }
            inline virtual ~YAPETException() throw() { /* empty */ }
            inline const YAPETException& operator= (const YAPETException& ex)
            throw() {
                if (this == &ex) return *this;

		exnum = ex.exnum;
                message = ex.message;
                return *this;
            }
            inline virtual const char* what() const throw() {
                return message.c_str();
            }
	    inline EXNUM getExNum() const throw() { return exnum; }
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
            inline YAPETRetryException()
            throw() : YAPETException (_ ("Retry") ) {}
            inline YAPETRetryException (std::string msg)
            throw() : YAPETException (msg) {}
            inline YAPETRetryException (const YAPETRetryException& ex)
            throw() : YAPETException (ex) {}
            inline virtual ~YAPETRetryException() throw() { /* Empty */ }

            inline const YAPETRetryException
            operator= (const YAPETRetryException& ex) throw() {
                if (this == &ex) return *this;

                YAPETException::operator= (ex);
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
            inline YAPETEncryptionException()
            throw() : YAPETException (_ ("Encryption error") ) {}
            inline YAPETEncryptionException (std::string msg)
            throw() : YAPETException (msg) {}
            inline YAPETEncryptionException (const YAPETEncryptionException& ex)
            throw() : YAPETException (ex) {}
            inline virtual ~YAPETEncryptionException() throw() { /* Empty */ }

            inline const YAPETEncryptionException
            operator= (const YAPETEncryptionException& ex) throw() {
                if (this == &ex) return *this;

                YAPETException::operator= (ex);
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
            inline YAPETInvalidPasswordException()
            throw() : YAPETException (_ ("Invalid password") ) {}
            inline YAPETInvalidPasswordException (std::string msg)
            throw() : YAPETException (msg) {}

            inline
            YAPETInvalidPasswordException (const YAPETInvalidPasswordException& ex)
            throw() : YAPETException (ex) {}
            inline virtual ~YAPETInvalidPasswordException()
            throw() { /* Empty */ }

            inline const YAPETInvalidPasswordException
            operator= (const YAPETInvalidPasswordException& ex) throw() {
                if (this == &ex) return *this;

                YAPETException::operator= (ex);
                return *this;
            }
    };

}

#endif // _YAPETEXCEPTION_H
