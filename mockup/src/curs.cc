// $Id$


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curs.h"
#include "dimension.h"

std::list<Window*> Curses::window_list;
std::string Curses::title;
WINDOW *Curses::w = NULL;
bool Curses::initialized = false;
bool Curses::hasstatusline = false;
bool Curses::hastitle = false;
int Curses::nlines=0, Curses::ncols=0, Curses::y=0, Curses::x=0;

/// Function object
class RefreshWindow {
    public:
	inline void operator()(Window *w) {
	    w->refresh();
	}
};

//
// Protected
//
void
Curses::putTitle() {
    int _h1 = ncols / 2;
    int _h2 = title.length() / 2;

    mvwaddstr(w, 0, _h1 - _h2, title.c_str() );
}


//
// Public
//
void
Curses::init() {
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
}

void
Curses::end() {
    if (!initialized) throw NotInitialized();

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

    std::for_each(window_list.begin(), window_list.end(), RefreshWindow());

    int retval = doupdate();
    if (retval == ERR)
	throw DoupdateFailed();
}

void
Curses::regwin(Window* W) {
    if (!initialized) throw NotInitialized();

    if (!W)
	throw std::invalid_argument("Argument cannot be null ptr");

    // New windows will be pushed at the back, so that they will be displayed
    // last and not obscured by earlier windows.
    window_list.push_back(W);
}

void
Curses::unregwin(Window* W) {
    // We do not check if Curses is still initialized, since that would impose
    // a strict order of how objects have to be destroyed, for instance, the
    // following code snippet would not work
    //
    // try {
    //  Curses::init()
    //  Window w1;
    //  ...
    //  Curses::end()
    // } catch (...) {
    //
    // because Curses will be ended before w1 is destroyed when leaving the
    // scope of the try-catch block
    window_list.remove(W);
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
