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

#ifndef _CHANGEPASSWORD_H
#define _CHANGEPASSWORD_H 1

#include <memory>

#include "mainwindow.h"
#include "newpassworddialog.h"
#include "passworddialog.h"
#include "yacurs.h"

/**
 * Handle changing password of YAPET file
 *
 * The glue class handling chage of YAPET file password.
 *
 * 1. if the currently open file has pending changes, it asks whether
 *    the changes should be saved, or the users wishes to cancel.
 *
 * 2. prompt for the old password, if it doesn't match show error
 *
 * 2. If old password matches, show new password prompt.
 */
class ChangePassword {
   private:
    MainWindow& mainwindow;
    PasswordDialog* promptoldpassword;
    NewPasswordDialog* promptpassword;
    YACURS::MessageBox2* nonmatch;
    YACURS::MessageBox3* confirmsave;
    YACURS::MessageBox2* generror;

    std::string _currentFilename;
    std::shared_ptr<yapet::AbstractCryptoFactory> _oldCryptoFactory;

    void window_close_handler(YACURS::Event& e);

   public:
    ChangePassword(MainWindow& mw);
    ChangePassword(const ChangePassword&) = delete;
    ChangePassword(ChangePassword&&) = delete;
    ChangePassword& operator=(const ChangePassword&) = delete;
    ChangePassword& operator=(ChangePassword&&) = delete;

    ~ChangePassword();

    void run();
};

#endif  // _CHANGEPASSWORD_H
