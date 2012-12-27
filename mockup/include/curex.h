// -*- mode: c++ -*-

#ifndef CUREX_H
#define CUREX_H

#include <exception>
#include <string>

class BaseCurEx : std::exception {
    private:
	std::string msg; 
	
    public:
	inline
	BaseCurEx(const char* m) throw () : msg(m) {}
	
	inline
	BaseCurEx(const BaseCurEx &c) throw() { msg = c.msg; }
	
	inline BaseCurEx& 
	operator=(const BaseCurEx &c) throw() {
	    if (this == &c)
		return *this;
	    
	    msg = c.msg;
	}
	
	inline virtual 
	~BaseCurEx() throw() {}
	
	inline virtual const char*
	what() const throw() { return msg.c_str(); }
};

class UnableToInitialize : BaseCurEx {
    public:
	UnableToInitialize() throw() :
	BaseCurEx("Unable to initialize curses") {}
};

class NotInitialized : BaseCurEx {
    public:
	NotInitialized() throw() :
	BaseCurEx("Curses not initialized") {}
};

class EndWinError : BaseCurEx {
    public:
	EndWinError() throw() :
	BaseCurEx("Endwin() had an error") {};
};

class AlreadyInitialized : BaseCurEx {
    public:
	AlreadyInitialized() throw() :
	BaseCurEx("Curses has already been initialized") {}
};

class RefreshFailed : BaseCurEx {
    public:
	RefreshFailed() throw() :
	BaseCurEx("Refresh failed") {}
};

class NewWindowFailed: BaseCurEx {
    public:
	NewWindowFailed() throw() :
	BaseCurEx("Creating new window failed") {}
};

class DelWindowFailed: BaseCurEx {
    public:
	DelWindowFailed() throw() :
	BaseCurEx("Deleting window failed") {}
};

#endif
