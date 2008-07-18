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

#ifndef _BASEWINDOW_H
#define _BASEWINDOW_H

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
    /**
     * @brief Base window class for windows wanting to receive resize events.
     *
     * Classes responding to window resize events should be derived from this
     * class.
     *
     * The constructor of this class registers the window to receive the resize
     * event. The destructor un-registers the class from the list of base
     * windows.
     *
     * There are also static members for dealing with signals and initializing
     * (n)curses.
     */
    class BaseWindow {
	public:
	    /**
	     * @brief Base class for calling a function upon the alarm signal.
	     *
	     * The class passed as caller for the action upon an alarm to \c
	     * setTimeout has to be derived from this class.
	     *
	     * The method \c AlarmFunction::process() will be called upon the
	     * \c SIGALRM signal.
	     */
	    class AlarmFunction {
		public:
		    inline virtual ~AlarmFunction() {}
		    /**
		     * @brief Called upon \c SIGALRM.
		     *
		     * This method is called upon a \c SIGALRM signal. The
		     * integer argument is the number of the signal, which
		     * currently is always \c SIGALRM.
		     */
		    virtual void process(int) = 0;
	    };
	private:
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static AlarmFunction* alarm_fun;
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    static std::list<BaseWindow*> basewindow_list;

	protected:
	    /**
	     * @brief Register a base window.
	     *
	     * This method will be called by the constructor of \c
	     * BaseWindow.
	     *
	     * @param r the pointer to the \c BaseWindow. Usually \c this.
	     */
	    static void registerBaseWindow(BaseWindow* r);
	    /**
	     * @brief un-registers a base window.
	     *
	     * This method will be called by the destructor of \c BaseWindow.
	     *
	     * @param r the pointer to the \c BaseWindow to be removed from the
	     * list. Usually \c this.
	     */
	    static void unregisterBaseWindow(BaseWindow* r);
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    /**
	     * @brief The signal handler.
	     *
	     * This is the signal handler for the signals processed.
	     *
	     * Upon \c SIGALRM (set by \c setTimeout()) it will call the \c
	     * process method of the \c AlarmFunction class pointed to be \c
	     * alarm_fun.
	     *
	     * @param signo the number of the signal.
	     */
	    static void sig_handler(int signo);
	    /**
	     * @brief Initializes the signal handlers.
	     *
	     * Initializes the signal handlers. This method will be called by
	     * \c initCurses().
	     */
	    static void init_signal();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

	    /**
	     * @brief The maximum x value of the screen.
	     *
	     * Returns the maximum x value of the screen.
	     *
	     * @return the maximum x value available to the curses functions.
	     */
	    inline int maxX() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_x;
	    }

	    /**
	     * @brief The maximum y value of the screen.
	     *
	     * Returns the maximum y value of the screen.
	     *
	     * @return the maximum y value available to the curses functions.
	     */
	    inline int maxY() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_y;
	    }

	    /**
	     * @brief The minimum x value of the screen.
	     *
	     * Returns the minimum x value of the screen.
	     *
	     * @return the minimum x value available to the curses functions.
	     */
	    inline int minX() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return x;
	    }

	    /**
	     * @brief The minimum y value of the screen.
	     *
	     * Returns the minimum y value of the screen.
	     *
	     * @return the minimum y value available to the curses functions.
	     */
	    inline int minY() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return y;
	    }
	public:
	    /**
	     * @brief The minimum dimensions supported
	     *
	     * Resize events below either of the dimensions will not
	     * be processed.
	     */
	    enum MinDimension {
		MIN_Y = 24,
		MIN_X = 80
	    };
	    /**
	     * @brief Initializes (n)curses.
	     *
	     * Initializes curses and sets up the signal handlers.
	     */
	    static void initCurses();
	    /**
	     * @brief Ends the curses mode.
	     *
	     * Ends the curses mode.
	     */
	    static void endCurses();
	    /**
	     * @brief Delete all registered windows.
	     *
	     * Deletes all registered windows by calling \c delete.
	     */
	    static void deleteAll();
	    /**
	     * @brief Calls the \c resize() method of all registered windows.
	     *
	     * Calls the \c resize() method of all registered windows.
	     */
	    static void resizeAll();
	    /**
	     * @brief Calls the \c refresh() method of all registered windows.
	     *
	     * Calls the \c refresh() method of all registered windows.
	     */
	    static void refreshAll();
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    /**
	     * @brief Sets a timeout.
	     *
	     * Sets a timeout using the system function \c alarm. Upon the \c
	     * SIGALRM signal, the \c process() method of the \c AlarmFunction
	     * class is called.
	     *
	     * @param af pointer to the \c AlarmFunction class.
	     *
	     * @param sec the number of seconds before \c SIGALRM is raised.
	     */
	    static void setTimeout(AlarmFunction* af, int sec);
	    /**
	     * @brief Suspends a currently set timeout.
	     *
	     * Suspends a currently set timeout.
	     */
	    static void suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    BaseWindow();
	    virtual ~BaseWindow();
	    virtual void resize() = 0;
	    virtual void refresh() = 0;
    };

}

#endif // _BASEWINDOW_H
