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

#ifndef _FILEOPEN_H
#define _FILEOPEN_H

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

#include <secstring.h>
#include <basewindow.h>
#include <listwidget.h>
#include <button.h>
#include <inputwidget.h>

/**
 * @brief Displays a "file open dialog" (sorta)
 *
 * Displays a window where the user can navigate the directory hierarchy and
 * select a file whose ending is \c .pet.
 *
 * @note The filename returned by \c getFilepath() always ends with \c .pet.
 */
class FileOpen : protected YAPET::UI::BaseWindow {
    private:
        enum {
            /**
             * @brief The max filename length.
             *
             * This is only used if we are unable to determine the max length
             * of the filename by other means.
             */
            FALLBACK_PATH_MAX = 255
        };

        std::string title;
        WINDOW* window;
        YAPET::UI::ListWidget<YAPET::UI::secstring>* dir;
        YAPET::UI::ListWidget<YAPET::UI::secstring>* files;
        YAPET::UI::InputWidget* input;
        YAPET::UI::Button* okbutton;
        YAPET::UI::Button* cancelbutton;

        bool canceled;

        inline FileOpen (const FileOpen&) {}
        inline const FileOpen& operator= (const FileOpen&) {
            return *this;
        }

        YAPET::UI::secstring directory;
        YAPET::UI::secstring filename;

        inline int windowWidth() {
            return maxX() - 8;
        }

        inline int windowHeight() {
            return maxY() - 4;
        }

        inline int startX() {
            return minX() + 4;
        }

        inline int startY() {
            return minY() + 2;
        }

        void createWindows() throw (YAPET::UI::UIException);

        void printTitle() throw (YAPET::UI::UIException);

        void printCWD() throw (YAPET::UI::UIException);

    protected:
        void getEntries (std::list<YAPET::UI::secstring>& d,
                         std::list<YAPET::UI::secstring>& f)
        throw (YAPET::UI::UIException);

        void getcwd() throw (YAPET::UI::UIException);
        void cd (const YAPET::UI::secstring d) throw (YAPET::UI::UIException);

    public:
        FileOpen (std::string t) throw (YAPET::UI::UIException);
        virtual ~FileOpen();

        void run() throw (YAPET::UI::UIException);
        void refresh() throw (YAPET::UI::UIException);

        void resize() throw (YAPET::UI::UIException);

        std::string getFilepath();

        inline bool isCanceled() const {
            return filename.empty() ? true : canceled;
        }
};

inline
bool endswith (YAPET::UI::secstring h, YAPET::UI::secstring n) {
    if (n.length() > h.length() )
        return false;

    if ( h.substr (h.length() - n.length(), n.length() ) == n)
        return true;

    return false;
}

inline
bool endswith (std::string h, std::string n) {
    if (n.length() > h.length() )
        return false;

    if ( h.substr (h.length() - n.length(), n.length() ) == n)
        return true;

    return false;
}

#endif // _FILEOPEN_H
