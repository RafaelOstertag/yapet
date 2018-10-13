/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifndef _CREATEFILE_H
#define _CREATEFILE_H 1

#include "mainwindow.h"
#include "newpassworddialog.h"
#include "yacurs.h"

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
    std::string _currentLoadedFile;
    std::string _filepath;
    bool ignore_unsaved_file;

    void window_close_handler(YACURS::Event& e);

   public:
    CreateFile(MainWindow& mw);
    CreateFile(const CreateFile&) = delete;
    CreateFile(const CreateFile&&) = delete;
    CreateFile& operator=(const CreateFile&) = delete;
    CreateFile& operator=(CreateFile&&) = delete;

    ~CreateFile();

    void run();
};

#endif  // _CREATEFILE_H
