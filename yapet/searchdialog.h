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

#ifndef _SEARCHDIALOG_H
#define _SEARCHDIALOG_H

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

#include <basewindow.h>
#include <button.h>
#include <inputwidget.h>

/**
 * @brief Search dialog
 *
 * Dialog for entering the search term
 */
class SearchDialog : protected YAPET::UI::BaseWindow {
    private:
        enum {
            HEIGHT = 6
        };

        WINDOW* window;
        YAPET::UI::InputWidget* searchtermw;
        YAPET::UI::Button* okbutton;
        YAPET::UI::Button* cancelbutton;
        YAPET::UI::secstring searchterm;

        bool canceled;

        inline SearchDialog (const SearchDialog&) {}
        inline const SearchDialog& operator= (const SearchDialog&) {
            return *this;
        }

        inline int getWidth() const {
            return maxX() - 8;
        }
        inline int getHeight() const {
            return HEIGHT;
        }

        inline int getStartX() const {
            return maxX() / 2 - getWidth() / 2;
        }
        inline int getStartY() const {
            return maxY() / 2 - getHeight() / 2;
        }

        void createWindow() throw (YAPET::UI::UIException);

    public:
        SearchDialog() throw (YAPET::UI::UIException);
        ~SearchDialog();

        void run() throw (YAPET::UI::UIException);
        void resize() throw (YAPET::UI::UIException);
        void refresh() throw (YAPET::UI::UIException);

        inline bool isCanceled() const {
            return canceled;
        }
        inline const char* getSearchTerm() const {
            return searchtermw->getText().c_str();
        }
};

#endif // _SEARCHDIALOG_H
