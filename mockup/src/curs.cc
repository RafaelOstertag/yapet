// $Id$


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif // HAVE_SIGNAL_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#else
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif // HAVE_TERMIOS_H
#endif // HAVE_SYS_TERMIOS_H

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "curs.h"
#include "eventqueue.h"

WINDOW* Curses::w = NULL;
StatusLine* Curses::statusline = NULL;
LineObject* Curses::title = NULL;
Window* Curses::mainwindow = NULL;
bool Curses::initialized = false;
Rectangle<> Curses::scrdim;

//
// Private
//

//
// Protected
//


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

    scrdim = inquiryScreenSize();

    EventQueue::registerHandler(EventConnectorFunction1(EVT_WINCH, resize));

    int retval;
#if !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))
    retval = wrefresh(stdscr);
    if (retval == ERR)
	throw RefreshFailed();
#endif // !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))

    retval = nonl();
    if (retval == ERR)
	throw NoNLFailed();

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

    Rectangle<> rmain(scrdim);


    if (title) {
	title->realize(Rectangle<>(0, 0, 1, scrdim.getCols()));

	rmain-=Margin<>(1,0,0,0);

	title->refresh();
    }

    if (statusline) {
	statusline->realize(Rectangle<>(scrdim.getLines()-1,0,1,
					scrdim.getCols()));
	rmain-=Margin<>(0,0,1,0);

	statusline->refresh();
    }

    if (mainwindow) {
	mainwindow->realize(rmain);

	mainwindow->refresh();
    }

    int retval = doupdate();
    if (retval == ERR)
	throw DoupdateFailed();
}

int
Curses::resize(EventBase& e) {
    if (!initialized) throw NotInitialized();

    Rectangle<> rmain(dynamic_cast<EventWinCh&>(e).data());

    if (title) {
	title->resize(Rectangle<>(0, 0, 1, scrdim.getCols()));

	rmain-=Margin<>(1,0,0,0);
    }

    if (statusline) {
	statusline->resize(Rectangle<>(scrdim.getLines()-1,0,1,
					scrdim.getCols()));
	rmain-=Margin<>(0,0,1,0);
    }

    if (mainwindow)
	mainwindow->resize(rmain);

    int retval = doupdate();
    if (retval == ERR)
	throw DoupdateFailed();

    return 0;
}

void
Curses::setTitle(LineObject* _title) {
    title = _title;
}

LineObject*
Curses::getTitle() {
    return title;
}

void
Curses::unsetTitle() {
    title = NULL;
}

void
Curses::setStatusLine(StatusLine* _sl) {
    statusline = _sl;
}

StatusLine*
Curses::getStatusLine() {
    return statusline;
}

void
Curses::unsetStatusLine() {
    statusline = NULL;
}

void
Curses::setWindow(Window* _w) {
    mainwindow = _w;
}

Window*
Curses::getWindow() {
    return mainwindow;
}

void
Curses::unsetWindow() {
    mainwindow = NULL;
}

Rectangle<>
Curses::inquiryScreenSize() {
    winsize ws;
    Rectangle<> __scrdim;

    __scrdim.setX(0);
    __scrdim.setY(0);

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
	if (ws.ws_row > 0 && ws.ws_col > 0) {
	    __scrdim.setLines(ws.ws_row);
	    __scrdim.setCols(ws.ws_col);
	} else {
	    throw WinSizeInvalid();
	}
    } else {
	char* clines = std::getenv("LINES");
	char* ccols = std::getenv("COLUMNS");

	if ( clines != NULL && ccols != NULL ) {
	    int _lines = std::atoi(clines);
	    int _cols = std::atoi(ccols);

	    if ( _lines > 0 && _cols > 0 ) {
		__scrdim.setLines(_lines);
		__scrdim.setCols(_cols);
	    } else {
		throw UnableToGetWinSize();
	    }
	} else {
	    throw UnableToGetWinSize();
	}
    }
    return __scrdim;

#if 0
    int x, y, nlines, ncols;
    getbegyx(w, y, x);
    getmaxyx(w, nlines, ncols);

    scrdim.setX(x);
    scrdim.setY(y);
    scrdim.setLines(nlines);
    scrdim.setCols(ncols);
#endif
}
