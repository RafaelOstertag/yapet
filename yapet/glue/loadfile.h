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

#ifndef _LOADFILE_H
#define _LOADFILE_H

#include "mainwindow.h"
#include "yacurs.h"
#include "promptpassword.h"

/**
 * 
 */
class LoadFile {
    private:
	MainWindow* mainwindow;
	PromptPassword* promptpassword;
	YACURS::FileLoadDialog* fileloaddialog;
	YACURS::MessageBox2* confirmsave;
	bool ignore_unsaved_file;

	LoadFile(const LoadFile&) {}

	const LoadFile& operator=(const LoadFile&) {
	    return *this;
	}

	void apoptosis_handler(YACURS::Event& e);

	void window_close_handler(YACURS::Event& e);

    public:
	LoadFile(MainWindow* mw);
	~LoadFile();

	void run();
};

#endif // _LOADFILE_H
