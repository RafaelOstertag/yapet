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

#ifndef _OPENCMDLINEFILE_H
#define _OPENCMDLINEFILE_H 1

#include "mainwindow.h"
#include "newpassworddialog.h"
#include "promptpassword.h"
#include "yacurs.h"

/**
 * Handle loading a file from command line.
 *
 * The class handles loading a file specified on the command line:
 *
 * 1. Check if file exists, if not prompt whether or not the file
 *    should be created.
 *
 * 2. If the file does not exist and should not be created stop.
 *
 * 3. If the file exists, prompt for password
 *
 * 4. If the file does not exist and should be created, prompt for new
 *    password.
 */
class LoadFileCmdLine {
   private:
    MainWindow& mainwindow;
    PromptPassword* promptpassword;
    NewPasswordDialog* newpassworddia;
    YACURS::MessageBox3* createfile;
    YACURS::MessageBox2* errormsg;
    YACURS::MessageBox2* generror;
    std::string _file;

    void apoptosis_handler(YACURS::Event& e);

    void window_close_handler(YACURS::Event& e);

   public:
    LoadFileCmdLine(MainWindow& mw, const std::string& file);
    LoadFileCmdLine(const LoadFileCmdLine&) = delete;
    LoadFileCmdLine(LoadFileCmdLine&&) = delete;
    LoadFileCmdLine& operator=(const LoadFileCmdLine&) = delete;
    LoadFileCmdLine& operator=(LoadFileCmdLine&&) = delete;
    ~LoadFileCmdLine();

    void run();
};

#endif  // _OPENCMDLINEFILE_H
