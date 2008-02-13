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

#include "resizeable.h"
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
	const Resizeable* ptr;

    public:
	inline RemoveByAddr(const Resizeable* p) : ptr(p) {}
	inline bool operator()(const Resizeable* p) const {
	    if (ptr == p)
		return true;
	    return false;
	}
};

class DeleteIt {
    public:
	inline void operator()(Resizeable* p) const {
	    if (p != NULL)
		delete p;
	}
};

class ResizeIt {
    public:
	inline void operator()(Resizeable* p) const {
	    p->resize();
	}
};

class RefreshIt {
    public:
	inline void operator()(Resizeable* p) const {
	    p->refresh();
	}
};

//
// Static
//
std::list<Resizeable*> Resizeable::resizeable_list = std::list<Resizeable*>();
Resizeable::AlarmFunction* Resizeable::alarm_fun = NULL;

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
void
Resizeable::sig_handler(int signo) {
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
Resizeable::init_signal() {
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
    sa.sa_handler = Resizeable::sig_handler;

    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGKILL, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
}
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

void
Resizeable::initCurses() {
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
Resizeable::endCurses() {
    clear();
    ::refresh();
    endwin();
}

void
Resizeable::registerResizeable(Resizeable* r) {
    resizeable_list.push_back(r);
}

void
Resizeable::unregisterResizeable(Resizeable* r) {
    std::list<Resizeable*>::iterator it =
	std::remove_if(resizeable_list.begin(),
		       resizeable_list.end(),
		       RemoveByAddr(r));

    resizeable_list.erase(it, resizeable_list.end());
}

void
Resizeable::deleteAll() {
    std::for_each(resizeable_list.rbegin(),
		  resizeable_list.rend(),
		  DeleteIt());
}

void
Resizeable::resizeAll() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    if (max_x<80 || max_y<25) return;
    std::for_each(resizeable_list.begin(),
		  resizeable_list.end(),
		  ResizeIt());
    refreshAll();
}

void
Resizeable::refreshAll() {
    std::for_each(resizeable_list.begin(),
		  resizeable_list.end(),
		  RefreshIt());
}

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
void
Resizeable::setTimeout(AlarmFunction* af, int sec) {
    alarm_fun = af;
    alarm(sec);
}

void
Resizeable::suspendTimeout() {
    alarm(0);
}
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)


//
// Non-Static
//
Resizeable::Resizeable() {
    Resizeable::registerResizeable(this);
}

Resizeable::~Resizeable() {
    Resizeable::unregisterResizeable(this);
}
