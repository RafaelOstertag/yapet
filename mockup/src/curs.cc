// $Id$


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "scrobjlist.h"
#include "curs.h"
#include "dimension.h"


std::string Curses::title;
WINDOW* Curses::w = NULL;
StatusLine* Curses::statusline;
bool Curses::initialized = false;
bool Curses::hasstatusline;
bool Curses::hastitle = false;
int Curses::nlines=0, Curses::ncols=0, Curses::y=0, Curses::x=0;



//
// Protected
//
void
Curses::putTitle() {
    int _h1 = ncols / 2;
    int _h2 = title.length() / 2;

    int retval = mymvwaddstr(w, 0, _h1 - _h2, title.c_str() );
    if (retval == ERR)
	throw AddStrFailed();
}


//
// Public
//
void
Curses::init(bool _statusline) {
    if (Curses::initialized)
	throw AlreadyInitialized();
    
    w = initscr();
    if (w == NULL)
	throw UnableToInitialize();

    getbegyx(w, y, x);
    getmaxyx(w, nlines, ncols);

#if !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))
    int retval = wrefresh(stdscr);
    if (retval == ERR)
	throw RefreshFailed();
#endif

    initialized = true;

    if (_statusline)
	statusline = new StatusLine();

    hasstatusline = _statusline;
}

void
Curses::end() {
    if (!initialized) throw NotInitialized();

    if (statusline)
	delete statusline;

    int retval = endwin();
    if (retval == ERR)
	throw EndWinError();

    initialized = false;
    w = NULL;
}

void
Curses::show() {
    if (!initialized) throw NotInitialized();

    if (hastitle) {
	putTitle();
	int retval = wnoutrefresh(w);
	if (retval == ERR)
	    throw RefreshFailed();
    }

    ScreenObjectList::refreshAll();


    int retval = doupdate();
    if (retval == ERR)
	throw DoupdateFailed();
}


void
Curses::setTitle(const std::string& t) {
    hastitle = true;
    title = t;
}

void
Curses::setTitle(const char* t) {
    hastitle = true;
    title = t;
}

std::string
Curses::getTitle() {
    return title;
}

void
Curses::unsetTitle() {
    title.clear();
    hastitle = false;
}

void
Curses::pushStatus(const std::string& m) {
    pushStatus(m.c_str());
}

void
Curses::pushStatus(const char* m) {
    if (!statusline) return;

    statusline->pushMsg(m);
}

void
Curses::popStatus() {
    if (!statusline) return;

    statusline->popMsg();
}

Dimension
Curses::getDimension() {
    if (!initialized) throw NotInitialized();

    int _y = y;
    int _nlines = nlines;
    if (hastitle) {
	_y++;
	_nlines--;
    }
    if (hasstatusline)
	_nlines--;

    return Dimension(_nlines, ncols, _y, x);
}

Dimension
Curses::getStatusDimension() {
    if (!initialized) throw NotInitialized();

    return Dimension(1,ncols,nlines-1,0);
}
