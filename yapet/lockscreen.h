// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2010  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _LOCKSCREEN_H
#define _LOCKSCREEN_H

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

#ifdef HAVE_STRING
# include <string>
#endif

#include <file.h>

#include "basewindow.h"

class LockScreen : protected YAPET::UI::BaseWindow {
    private:
        WINDOW* lockwin;
        //! Used to indicate handle_signal() that the user wishes to quit from
        //! the lock screen
        volatile bool do_quit;
        bool dont_allow_quit;
        //! indicate whether or not a resize happened during the runtime of the
        //! screen lock.
        volatile bool resize_due;
        const YAPET::Key* key;
        const YAPET::File* file;

        LockScreen(const LockScreen&) {
            assert(0);
        }

        const LockScreen& operator=(const LockScreen&) {
            assert(0); return *this;
        }

    public:
        LockScreen(const YAPET::Key* k, const YAPET::File* f, bool daq);
        ~LockScreen();
        void run() throw(YAPET::UI::UIException);

        virtual void resize();

        virtual void refresh();

        inline bool getResizeDue() const {
            return resize_due;
        }

        inline bool getDoQuit() const {
            return do_quit;
        }
};

#endif // _LOCKSCREEN_H
