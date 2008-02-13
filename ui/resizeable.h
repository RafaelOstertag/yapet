// -*- c++ -*-
//
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

#ifndef _RESIZEABLE_H
#define _RESIZEABLE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_LIST
# include <list>
#endif

namespace YAPETUI {
  
    class Resizeable {
	public:
	    class AlarmFunction {
		public:
		    inline AlarmFunction() {}
		    inline virtual ~AlarmFunction() {}
		    inline virtual void process(int) {}
	    };
	private:
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static AlarmFunction* alarm_fun;
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static std::list<Resizeable*> resizeable_list;

	protected:
	    static void registerResizeable(Resizeable* r);
	    static void unregisterResizeable(Resizeable* r);
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static void sig_handler(int signo);
	    static void init_signal();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

	    inline int maxX() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_x;
	    }

	    inline int maxY() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_y;
	    }

	    inline int minX() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return x;
	    }

	    inline int minY() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return y;
	    }
	public:
	    static void initCurses();
	    static void endCurses();
	    static void deleteAll();
	    static void resizeAll();
	    static void refreshAll();
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static void setTimeout(AlarmFunction* af, int sec);
	    static void suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    Resizeable();
	    virtual ~Resizeable();
	    virtual void resize() = 0;
	    virtual void refresh() = 0;
    };

}

#endif // _RESIZEABLE_H
