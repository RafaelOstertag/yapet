// -*- c++ -*-
//
// $Id$

#ifndef _GPSEXCEPTION_H
#define _GPSEXCEPTION_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <exception>
#include <string>

namespace GPSAFE {

    class GPSException : public std::exception {
	private:
	    std::string message;
	    
	public:
	    inline GPSException() throw() : exception(),
					    message("Generic exception message") {}
	    inline GPSException(std::string msg) throw() : exception(),
						   message(msg) {}
	    inline GPSException(const GPSException& ex) throw() {
		message = ex.message;
	    }
	    inline virtual ~GPSException() throw() { /* empty */ }
	    inline const GPSException& operator=(const GPSException& ex) throw() {
		if (this == &ex) return *this;
		message = ex.message;
		return *this;
	    }
	    inline virtual const char* what() const throw() { return message.c_str(); }
    };    

    class GPSRetryException : public GPSException {
	public:
	    inline GPSRetryException() throw() : GPSException("Retry") {}
	    inline GPSRetryException(std::string msg) throw() : GPSException(msg) {}
	    inline GPSRetryException(const GPSRetryException& ex) throw() : GPSException(ex) {}
	    inline virtual ~GPSRetryException() throw() { /* Empty */ }
	    inline const GPSRetryException operator=(const GPSRetryException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
		return *this;
	    }
    };

    class GPSEncryptionException : public GPSException {
	public:
	    inline GPSEncryptionException() throw() : GPSException("Encryption error") {}
	    inline GPSEncryptionException(std::string msg) throw() : GPSException(msg) {}
	    inline GPSEncryptionException(const GPSEncryptionException& ex) throw() : GPSException(ex) {}
	    inline virtual ~GPSEncryptionException() throw() { /* Empty */ }
	    inline const GPSEncryptionException operator=(const GPSEncryptionException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
		return *this;
	    }
    };

    class GPSInvalidPasswordException : public GPSException {
	public:
	    inline GPSInvalidPasswordException() throw() : GPSException("Invalid password") {}
	    inline GPSInvalidPasswordException(std::string msg) throw() : GPSException(msg) {}
	    inline GPSInvalidPasswordException(const GPSInvalidPasswordException& ex) throw() : GPSException(ex) {}
	    inline virtual ~GPSInvalidPasswordException() throw() { /* Empty */ }
	    inline const GPSInvalidPasswordException operator=(const GPSInvalidPasswordException& ex) throw() {
		if (this == &ex) return *this;
		GPSException::operator=(ex);
		return *this;
	    }
    };
						 
}
#endif // _GPSEXCEPTION_H
