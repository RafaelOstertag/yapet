// -*- c++ -*-
//
// $Id$
//
// @@REPLACE@@
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

#ifndef _GPSEXCEPTION_H
#define _GPSEXCEPTION_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_EXCEPTION
# include <exception>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

namespace GPSAFE {

    /**
     * @brief The exception class used for cryptographic classes.
     *
     * The exception base class used for cryptographic classes.
     */
    class GPSException : public std::exception {
	private:
	    std::string message;
	    
	public:
	    /**
	     * @brief Initializes a default exception message
	     *
	     * Initializes a default exception message
	     */
	    inline GPSException() 
		throw() : exception(),
			  message("Generic exception message") {}
	    /**
	     * @brief Initializes with a user specified message
	     *
	     * Initializes with a user specified message
	     *
	     * @param msg the message of the exception
	     */
	    inline GPSException(std::string msg) 
		throw() : exception(),
			  message(msg) {}
	    inline GPSException(const GPSException& ex) throw() {
		message = ex.message;
	    }
	    inline virtual ~GPSException() throw() { /* empty */ }
	    inline const GPSException& operator=(const GPSException& ex)
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
    class GPSRetryException : public GPSException {
	public:
	    inline GPSRetryException() 
		throw() : GPSException("Retry") {}
	    inline GPSRetryException(std::string msg)
		throw() : GPSException(msg) {}
	    inline GPSRetryException(const GPSRetryException& ex)
		throw() : GPSException(ex) {}
	    inline virtual ~GPSRetryException() throw() { /* Empty */ }

	    inline const GPSRetryException
	    operator=(const GPSRetryException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
		return *this;
	    }
    };

    /**
     * @brief Indicates an error while encrypting/decrypting.
     *
     * This function indicates an error while encrypting/decrypting
     * data.
     */
    class GPSEncryptionException : public GPSException {
	public:
	    inline GPSEncryptionException()
		throw() : GPSException("Encryption error") {}
	    inline GPSEncryptionException(std::string msg)
		throw() : GPSException(msg) {}
	    inline GPSEncryptionException(const GPSEncryptionException& ex)
		throw() : GPSException(ex) {}
	    inline virtual ~GPSEncryptionException() throw() { /* Empty */ }

	    inline const GPSEncryptionException
	    operator=(const GPSEncryptionException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
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
    class GPSInvalidPasswordException : public GPSException {
	public:
	    inline GPSInvalidPasswordException()
		throw() : GPSException("Invalid password") {}
	    inline GPSInvalidPasswordException(std::string msg)
		throw() : GPSException(msg) {}

	    inline
	    GPSInvalidPasswordException(const GPSInvalidPasswordException& ex)
		throw() : GPSException(ex) {}
	    inline virtual ~GPSInvalidPasswordException()
		throw() { /* Empty */ }

	    inline const GPSInvalidPasswordException
	    operator=(const GPSInvalidPasswordException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
		return *this;
	    }
    };
						 
}

#endif // _GPSEXCEPTION_H
