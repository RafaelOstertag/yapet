// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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
#include <listbox.h>

/**
 */
class MainWindow : public YACURS::Window {
    private:
        bool records_changed;

        YACURS::ListBox<YAPET::PartDec>* recordlist;

        bool usefsecurity;

        MainWindow(const MainWindow&) {
        }

        const MainWindow& operator=(const MainWindow&) {
            return *this;
        }

    protected:
        void createFile(std::string& filename);

        void openFile(std::string filename);

        void saveFile();

        void closeFile();

        void addNewRecord();

        void editSelectedRecord();

        void deleteSelectedRecord();

        void setSortOrder();

        void searchTerm();

        void searchNext();

        bool quit();

        void changePassword();

    public:
        MainWindow();
        virtual ~MainWindow();
};

#endif // _MAINWINDOW_H
