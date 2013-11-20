// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2013  Rafael Ostertag
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

#ifndef _CREATEFILE_H
#define _CREATEFILE_H 1

#include "mainwindow.h"
#include "yacurs.h"
#include "newpassworddialog.h"

/**
 * Handle creating a file.
 *
 * The class handles creating a file:
 *
 * 1. if the currently open file has pending changes, it asks whether
 *    the changes should be saved, or the users wishes to cancel.
 *
 * 2. Show save dialog.
 *
 * 3. If save dialog was successful, show new password prompt.
 */
class CreateFile {
    private:
	MainWindow& mainwindow;
	NewPasswordDialog* promptpassword;
	YACURS::FileSaveDialog* filesavedialog;
	YACURS::MessageBox2* confirmsave;
	YACURS::MessageBox2* generror;
	std::string __filepath;
	bool ignore_unsaved_file;

	CreateFile(const CreateFile& c) : mainwindow(c.mainwindow) {}

	const CreateFile& operator=(const CreateFile&) {
	    return *this;
	}

	void window_close_handler(YACURS::Event& e);

    public:
	CreateFile(MainWindow& mw);
	~CreateFile();

	void run();
};

#endif // _CREATEFILE_H
