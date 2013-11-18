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
#define _MAINWINDOW_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Crypt
#include <file.h>

#include <yacurs.h>

#include "help.h"
#include "info.h"
#include "passworddialog.h"
#include "passwordrecord.h"

/**
 */
class MainWindow : public YACURS::Window {
    private:
	YACURS::ListBox<YAPET::PartDec>* recordlist;
	HelpDialog* helpdialog;
	InfoDialog* infodialog;
	YACURS::MessageBox* confirmdelete;
	YACURS::MessageBox2* confirmquit;
	PasswordRecord* passwordrecord;
	YACURS::MessageBox2* errormsgdialog;
	// Used when opening an existing record or deleting a record
	// to store the index, so that it can be reselect in case of a
	// window resize.
	YACURS::ListBox<YAPET::PartDec>::lsz_t record_index;


        MainWindow(const MainWindow&) {
        }

        const MainWindow& operator=(const MainWindow&) {
            return *this;
        }

	void apoptosis_handler(YACURS::Event& e);

	void window_close_handler(YACURS::Event& e);

	void listbox_enter_handler(YACURS::Event& e);

    public:
        MainWindow();
        virtual ~MainWindow();

	/**
	 * Load password file.
	 *
	 * Unconditionally load a password file, i.e. changes not
	 * committed to disk on an already shown file will be lost.
	 *
	 * @param file YAPET file
	 *
	 * @param key corresponding key to @c file
	 */
	void load_password_file(YAPET::File* file, YAPET::Key* key);

	/**
	 * @param selected whether or not to preload the dialog with
	 * the currently selected password record.
	 */
	void show_password_record(bool selected);

	void delete_selected();

	void save_records();

	void change_password(YAPET::Key* nk);

	void show_help();

	void show_info();

	void quit();

	/**
	 * Get the number of password records.
	 */
	YACURS::ListBox<>::lsz_t size() {
	    return recordlist->list().size();
	}

	void sort_asc(bool f);
	bool sort_asc() const;
};

#endif // _MAINWINDOW_H
