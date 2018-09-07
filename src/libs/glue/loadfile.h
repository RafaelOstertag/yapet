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
#define _LOADFILE_H 1

#include "mainwindow.h"
#include "promptpassword.h"
#include "yacurs.h"

/**
 * Handle loading a file.
 *
 * The class handles loading a file:
 *
 * 1. if the currently open file has pending changes, it asks whether
 *    the changes should be saved, or the users wishes to cancel.
 *
 * 2. If the user did not cancel, or no pending changes required a
 *    prompt, displays a file load dialog.
 *
 * 3. If the file dialog has not been cancelled, it uses
 *    PromptPassword in order to obtain a password for the file. If
 *    the password is successfully obtained,
 *    MainWindow::load_password_file() is called.
 */
class LoadFile {
   private:
    MainWindow& mainwindow;
    PromptPassword* promptpassword;
    YACURS::FileLoadDialog* fileloaddialog;
    YACURS::MessageBox2* confirmsave;
    std::string _selectedFilename;
    std::string _currentLoadedFile;
    bool ignore_unsaved_file;

    void apoptosis_handler(YACURS::Event& e);

    void window_close_handler(YACURS::Event& e);

   public:
    LoadFile(MainWindow& mw);
    LoadFile(const LoadFile& l) = delete;
    LoadFile& operator=(const LoadFile&) = delete;

    ~LoadFile();

    void run();
};

#endif  // _LOADFILE_H
