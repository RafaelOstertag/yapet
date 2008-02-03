// -*- c++ -*-
//
// $Id$

#ifndef _UIEXCEPTION_H
#define _UIEXCEPTION_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <exception>
#include <string>

namespace GPSUI {

    class UIException : public std::exception {
	private:
	    std::string message;
	    
	public:
	    inline UIException() throw() : exception(),
					   message("Generic UI exception") {}
	    inline UIException(std::string msg) throw() : exception(),
							  message(msg) {}
	    inline UIException(const UIException& ex) throw() {
		message = ex.message;
	    }
	    inline virtual ~UIException() throw() { /* empty */ }
	    inline const UIException& operator=(const UIException& ex) throw() {
		if (this == &ex) return *this;
		message = ex.message;
		return *this;
	    }
	    inline virtual const char* what() const throw() { return message.c_str(); }
    };

}

#endif // _UIEXCEPTION_H
