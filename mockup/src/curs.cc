// $Id$


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <signal.h>
#include <cerrno>
#include <cstdlib>

#include "curs.h"

sigset_t Curses::sigoldmask;
struct sigaction Curses::sigoldaction;

WINDOW* Curses::w = NULL;
StatusLine* Curses::statusline = NULL;
LineObject* Curses::title = NULL;
Window* Curses::mainwindow = NULL;
bool Curses::initialized = false;
Rectangle<> Curses::scrdim;

//
// Private
//
void
Curses::setupSignal() {
#ifdef SIGWINCH
    struct sigaction winchact;

    winchact.sa_sigaction = signal_handler;
    winchact.sa_flags = SA_SIGINFO;
    sigemptyset(&winchact.sa_mask);

    int err = sigaction(SIGWINCH, &winchact, &sigoldaction);
    if (err)
	throw SystemError(errno);

    sigset_t nset;

    sigemptyset(&nset);
    sigaddset(&nset, SIGWINCH);

    err = sigprocmask(SIG_UNBLOCK, &nset, &sigoldmask);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH

}

void
Curses::restoreSignal() {
#ifdef SIGWINCH
    int err = sigaction(SIGWINCH, &sigoldaction, NULL);
    if (err)
	throw SystemError(errno);

    err = sigprocmask(SIG_SETMASK, &sigoldmask, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH
}

void
Curses::signal_handler(int signo, siginfo_t* info, void *d) {
    int olderrno = errno;

    Curses::resize();

    errno = olderrno;
}

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

    int x, y, nlines, ncols;
    getbegyx(w, y, x);
    getmaxyx(w, nlines, ncols);

    scrdim.setX(x);
    scrdim.setY(y);
    scrdim.setLines(nlines);
    scrdim.setCols(ncols);

#if !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))
    int retval = wrefresh(stdscr);
    if (retval == ERR)
	throw RefreshFailed();
#endif

    setupSignal();

    initialized = true;
}

void
Curses::end() {
    if (!initialized) throw NotInitialized();

    restoreSignal();

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

void
Curses::resize() {
    if (!initialized) throw NotInitialized();

    int x, y, nlines, ncols;
    getbegyx(w, y, x);
    getmaxyx(w, nlines, ncols);

    scrdim.setX(x);
    scrdim.setY(y);
    scrdim.setLines(nlines);
    scrdim.setCols(ncols);

    show();
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

