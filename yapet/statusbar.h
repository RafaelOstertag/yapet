// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#ifndef _STATUSBAR_H
#define _STATUSBAR_H

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

#include "basewindow.h"
#include "uiexception.h"

/**
 * @brief A status bar at the bottom of the screen.
 *
 * Shows a status bar at the bottom of the screen on character high. Messages
 * can be showed to the user by calling \c putMsg(). The message stays visible
 * until a new message is put.
 */
class StatusBar : protected YAPET::UI::BaseWindow {
    private:
        WINDOW* statusbar;
        std::string message;

        inline StatusBar (const StatusBar&) {}
        inline const StatusBar& operator= (const StatusBar&) {
            return *this;
        }

    protected:
        void createWindow() throw (YAPET::UI::UIException);

    public:
        StatusBar() throw (YAPET::UI::UIException);
        virtual ~StatusBar();

        void putMsg (std::string msg) throw (YAPET::UI::UIException);
        std::string getMsg() const {
            return message;
        }
        void refresh();

        void resize();
};

#endif // _STATUSBAR_H
