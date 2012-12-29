// -*- mode: c++ -*-

#ifndef CUREX_H
#define CUREX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <exception>
#include <string>

class BaseCurEx: public std::exception {
    private:
	std::string msg; 
	
    public:
	inline
	BaseCurEx(const char* m) : msg(m) {}
	
	inline
	BaseCurEx(const BaseCurEx &c) { msg = c.msg; }
	
	inline BaseCurEx& 
	operator=(const BaseCurEx &c) {
	    if (this == &c)
		return *this;
	    
	    msg = c.msg;
	}
	
	inline virtual 
	~BaseCurEx() throw() {}
	
	inline virtual const char*
	what() const throw() { return msg.c_str(); }
};

class UnableToInitialize: public BaseCurEx {
    public:
	UnableToInitialize() :
	BaseCurEx("Unable to initialize curses") {}
};

class NotInitialized: public BaseCurEx {
    public:
	NotInitialized() :
	BaseCurEx("Curses not initialized") {}
};

class EndWinError: public BaseCurEx {
    public:
	EndWinError() :
	BaseCurEx("Endwin() had an error") {};
};

class AlreadyInitialized: public BaseCurEx {
    public:
	AlreadyInitialized() :
	BaseCurEx("Curses has already been initialized") {}
};

class RefreshFailed: public BaseCurEx {
    public:
	RefreshFailed() :
	BaseCurEx("Refresh failed") {}
};

class NewWindowFailed: public BaseCurEx {
    public:
	NewWindowFailed() :
	BaseCurEx("Creating new window failed") {}
};

class DelWindowFailed: public BaseCurEx {
    public:
	DelWindowFailed() :
	BaseCurEx("Deleting window failed") {}
};

class BoxFailed: public BaseCurEx {
    public:
	BoxFailed() :
	BaseCurEx("Creating box (border) on window failed") {}
};

class DoupdateFailed: public BaseCurEx {
    public:
	DoupdateFailed() :
	BaseCurEx("doupdate() failed") {}
};

#endif
