// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#include "curex.h"
#include <curses.h>

class Curses {
    private:
	static WINDOW *w;
	static bool initialized;
    
    public:
	static void init() throw (AlreadyInitialized, UnableToInitialize, RefreshFailed);
	static void end() throw (NotInitialized,EndWinError);
};

#endif
