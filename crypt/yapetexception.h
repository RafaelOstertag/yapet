// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _YAPETEXCEPTION_H
#define _YAPETEXCEPTION_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_EXCEPTION
# include <exception>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

namespace YAPET {

    /**
     * @brief The exception class used for cryptographic classes.
     *
     * The exception base class used for cryptographic classes.
     */
    class YAPETException : public std::exception {
	private:
	    std::string message;
	    
	public:
	    /**
	     * @brief Initializes a default exception message
	     *
	     * Initializes a default exception message
	     */
	    inline YAPETException() 
		throw() : exception(),
			  message("Generic exception message") {}
	    /**
	     * @brief Initializes with a user specified message
	     *
	     * Initializes with a user specified message
	     *
	     * @param msg the message of the exception
	     */
	    inline YAPETException(std::string msg) 
		throw() : exception(),
			  message(msg) {}
	    inline YAPETException(const YAPETException& ex) throw() {
		message = ex.message;
	    }
	    inline virtual ~YAPETException() throw() { /* empty */ }
	    inline const YAPETException& operator=(const YAPETException& ex)
		throw() {
		if (this == &ex) return *this;
		message = ex.message;
		return *this;
	    }
	    inline virtual const char* what() const throw() {
		return message.c_str();
	    }
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
		throw() : YAPETException("Retry") {}
	    inline YAPETRetryException(std::string msg)
		throw() : YAPETException(msg) {}
	    inline YAPETRetryException(const YAPETRetryException& ex)
		throw() : YAPETException(ex) {}
	    inline virtual ~YAPETRetryException() throw() { /* Empty */ }

	    inline const YAPETRetryException
	    operator=(const YAPETRetryException& ex) throw() {
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
	    inline YAPETEncryptionException()
		throw() : YAPETException("Encryption error") {}
	    inline YAPETEncryptionException(std::string msg)
		throw() : YAPETException(msg) {}
	    inline YAPETEncryptionException(const YAPETEncryptionException& ex)
		throw() : YAPETException(ex) {}
	    inline virtual ~YAPETEncryptionException() throw() { /* Empty */ }

	    inline const YAPETEncryptionException
	    operator=(const YAPETEncryptionException& ex) throw() {
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
	    inline YAPETInvalidPasswordException()
		throw() : YAPETException("Invalid password") {}
	    inline YAPETInvalidPasswordException(std::string msg)
		throw() : YAPETException(msg) {}

	    inline
	    YAPETInvalidPasswordException(const YAPETInvalidPasswordException& ex)
		throw() : YAPETException(ex) {}
	    inline virtual ~YAPETInvalidPasswordException()
		throw() { /* Empty */ }

	    inline const YAPETInvalidPasswordException
	    operator=(const YAPETInvalidPasswordException& ex) throw() {
		if (this == &ex) return *this;
		YAPETException::operator=(ex);
		return *this;
	    }
    };
						 
}

#endif // _YAPETEXCEPTION_H
