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

#ifndef _CHANGEPASSWORD_H
#define _CHANGEPASSWORD_H

#include "mainwindow.h"
#include "yacurs.h"
#include "newpassworddialog.h"

/**
 * Handle changing password of YAPET file
 *
 * The glue class handling chage of YAPET file password.
 *
 * 1. if the currently open file has pending changes, it asks whether
 *    the changes should be saved, or the users wishes to cancel.
 *
 * 2. If confirmation dialog was successful, show new password prompt.
 */
class ChangePassword {
    private:
	MainWindow* mainwindow;
	NewPasswordDialog* promptpassword;
	YACURS::MessageBox3* confirmsave;
	YACURS::MessageBox2* generror;
	std::string __filepath;
	bool ignore_unsaved_file;

	ChangePassword(const ChangePassword&) {}

	const ChangePassword& operator=(const ChangePassword&) {
	    return *this;
	}

	void window_close_handler(YACURS::Event& e);

    public:
	ChangePassword(MainWindow* mw);
	~ChangePassword();

	void run();
};

#endif // _CHANGEPASSWORD_H
