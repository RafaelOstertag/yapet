// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H


#ifdef HAVE_CONFIG_H
#include <config.h>
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

#include <file.h>
#include <basewindow.h>
#include <uiexception.h>
#include <statusbar.h>
#include <listwidget.h>

/**
 * @brief The main window.
 *
 * This is the main window class which shows the main menu in top right window,
 * some information in the lower right window and the passwords stored in the
 * file currently open in the left window.
 *
 * It handles the menu key strokes and provides a screen locking function which
 * is called after a certain number of seconds using the \c
 * BaseWindow::setTimeout() method.
 */
class MainWindow : protected YAPET::UI::BaseWindow {
    private:
        WINDOW* toprightwin;
        WINDOW* bottomrightwin;
        YAPET::UI::ListWidget<YAPET::PartDec>* recordlist;
        StatusBar statusbar;
        bool records_changed;

        YAPET::Key* key;
        YAPET::File* file;

        unsigned int locktimeout;

        bool usefsecurity;

        inline MainWindow (const MainWindow&) {}
        inline const MainWindow& operator= (const MainWindow&) {
            return *this;
        }

    protected:
        void printTitle() throw (YAPET::UI::UIException);

        void topRightWinContent() throw (YAPET::UI::UIException);

        void bottomRightWinContent() throw (YAPET::UI::UIException);

        void createWindow() throw (YAPET::UI::UIException);

        void refresh() throw (YAPET::UI::UIException);

        void createFile (std::string& filename) throw (YAPET::UI::UIException);
        void openFile (std::string filename) throw (YAPET::UI::UIException);
        void saveFile();
        void closeFile();

        void addNewRecord();
        void editSelectedRecord();
        void deleteSelectedRecord() throw (YAPET::UI::UIException);
        void setSortOrder();
        void searchTerm();
        void searchNext();
        bool quit();
        void lockScreen() const throw (YAPET::UI::UIException);
        void changePassword() throw (YAPET::UI::UIException);
    public:
        MainWindow (unsigned int timeout, bool fsecurity) throw (YAPET::UI::UIException);
        virtual ~MainWindow();

        void run() throw (YAPET::UI::UIException);
        void run (std::string fn);
        void resize() throw (YAPET::UI::UIException);

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        void handle_signal (int signo);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
};

#endif // _MAINWINDOW_H
