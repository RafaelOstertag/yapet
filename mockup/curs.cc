#include "curs.h"

bool
Curses::initialized = false;
WINDOW
*Curses::w = NULL;

void
Curses::init() throw (AlreadyInitialized, UnableToInitialize, RefreshFailed) {
    if (Curses::initialized)
	throw AlreadyInitialized();
    
    w = initscr();
    if (w == NULL)
	throw UnableToInitialize();

    int retval = refresh();
    if (retval == ERR)
	throw RefreshFailed();

    initialized = true;
}

void
Curses::end() throw (NotInitialized,EndWinError) {
    if (!initialized)
	throw NotInitialized();

    int retval = endwin();
    if (retval == ERR)
	throw EndWinError();

    initialized = false;
    w = NULL;
}
