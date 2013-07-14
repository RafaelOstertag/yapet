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

#include <yacurs.h>

#include "help.h"
#include "passworddialog.h"

/**
 */
class MainWindow : public YACURS::Window {
    private:
	YACURS::ListBox<YAPET::PartDec>* recordlist;
	HelpDialog* helpdialog;
	PasswordDialog* passworddialog;
	YACURS::MessageBox* errormsgdialog;
	YACURS::MessageBox2* closeconfirmdialog;
	YACURS::FileLoadDialog* fileopendialog;

	std::string filename;

        MainWindow(const MainWindow&) {
        }

        const MainWindow& operator=(const MainWindow&) {
            return *this;
        }

	void save_and_close_petfile();
	void close_petfile();

	/**
	 * @param checkchanges if \c true, check whether or not there
	 * are changes pending is check. If changes are pending, a
	 * dialog will be displayed asking whether or not changes
	 * should be saved.
	 */
	void ui_close_pet_file(bool checkchanges=true);

	void window_close_handler(YACURS::Event& e);

    public:
        MainWindow();
        virtual ~MainWindow();

	void show_help();
	void show_file_open();
};

#endif // _MAINWINDOW_H
