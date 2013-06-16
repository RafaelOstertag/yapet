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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Crypt
#include <file.h>

// YACURS
#include <window.h>
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
class MainWindow : public YACURS::Window {
    private:
        bool records_changed;

	YACURS::ListWidget<YAPET::PartDec>* recordlist;
        YAPET::Key* key;
        YAPET::File* file;

        bool usefsecurity;

        MainWindow (const MainWindow&) {}
        const MainWindow& operator= (const MainWindow&) {
            return *this;
        }

    protected:
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
        void changePassword() throw (YAPET::UI::UIException);
    public:
        MainWindow ()
        virtual ~MainWindow();
};

#endif // _MAINWINDOW_H
