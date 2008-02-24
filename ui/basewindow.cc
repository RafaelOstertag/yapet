// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "basewindow.h"
#include "colors.h"

#ifdef HAVE_SIGNAL_H
# include <signal.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_ALGORITHM
# include <algorithm>
#endif

using namespace YAPETUI;

class RemoveByAddr {
    private:
	const BaseWindow* ptr;

    public:
	inline RemoveByAddr(const BaseWindow* p) : ptr(p) {}
	inline bool operator()(const BaseWindow* p) const {
	    if (ptr == p)
		return true;
	    return false;
	}
};

class DeleteIt {
    public:
	inline void operator()(BaseWindow* p) const {
	    if (p != NULL)
		delete p;
	}
};

class ResizeIt {
    public:
	inline void operator()(BaseWindow* p) const {
	    p->resize();
	}
};

class RefreshIt {
    public:
	inline void operator()(BaseWindow* p) const {
	    p->refresh();
	}
};

//
// Static
//
std::list<BaseWindow*> BaseWindow::basewindow_list = std::list<BaseWindow*>();
BaseWindow::AlarmFunction* BaseWindow::alarm_fun = NULL;

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
void
BaseWindow::sig_handler(int signo) {
    switch (signo) {
    case SIGALRM:
	if (alarm_fun != NULL)
	    alarm_fun->process(signo);
	break;
    case SIGHUP:
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
    case SIGKILL:
	deleteAll();
	endCurses();
	abort();
    }
}

void
BaseWindow::init_signal() {
    sigset_t sigset;
    sigemptyset(&sigset);
    // Get the current sigprocmask
    sigprocmask(SIG_SETMASK, NULL, &sigset);
    // enable the signals we want
    sigaddset(&sigset, SIGALRM);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGKILL);
    sigaddset(&sigset, SIGQUIT);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGHUP);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = BaseWindow::sig_handler;

    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGKILL, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
}
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

void
BaseWindow::initCurses() {
    initscr();
    raw();
    noecho();
    ::refresh();
    curs_set(0);
    keypad (stdscr, TRUE);

    YAPETUI::Colors::initColors();
    init_signal();
}

void
BaseWindow::endCurses() {
    clear();
    ::refresh();
    endwin();
}

void
BaseWindow::registerBaseWindow(BaseWindow* r) {
    basewindow_list.push_back(r);
}

void
BaseWindow::unregisterBaseWindow(BaseWindow* r) {
    std::list<BaseWindow*>::iterator it =
	std::remove_if(basewindow_list.begin(),
		       basewindow_list.end(),
		       RemoveByAddr(r));

    basewindow_list.erase(it, basewindow_list.end());
}

void
BaseWindow::deleteAll() {
    std::for_each(basewindow_list.rbegin(),
		  basewindow_list.rend(),
		  DeleteIt());
}

void
BaseWindow::resizeAll() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    std::for_each(basewindow_list.begin(),
		  basewindow_list.end(),
		  ResizeIt());
    refreshAll();
}

void
BaseWindow::refreshAll() {
    std::for_each(basewindow_list.begin(),
		  basewindow_list.end(),
		  RefreshIt());
}

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
void
BaseWindow::setTimeout(AlarmFunction* af, int sec) {
    alarm_fun = af;
    alarm(sec);
}

void
BaseWindow::suspendTimeout() {
    alarm(0);
}
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

//
// Non-Static
//
BaseWindow::BaseWindow() {
    BaseWindow::registerBaseWindow(this);
}

BaseWindow::~BaseWindow() {
    BaseWindow::unregisterBaseWindow(this);
}
