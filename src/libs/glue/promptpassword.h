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

#ifndef _PROMPTPASSWORD_H
#define _PROMPTPASSWORD_H 1

#include <memory>

#include "abstractcryptofactory.hh"
#include "passworddialog.h"
#include "yacurs.h"

/**
 * Prompt for a password and try to open a given file.
 *
 * Prompt for a password to open a given file. If the password does
 * not match on the file, ask the user whether to try again or cancel.
 *
 *
 * Once the object has done it's job, it submits a EVT_APOPTOSIS
 * event.
 */
class PromptPassword {
   private:
    PasswordDialog* pwdialog;
    YACURS::MessageBox3* pwerror;
    YACURS::MessageBox2* generror;
    std::string _filename;

    std::shared_ptr<yapet::AbstractCryptoFactory> _cryptoFactory;

    void window_close_handler(YACURS::Event& e);

   public:
    /**
     * @param fn the filename to prompt password for.
     */
    PromptPassword(const std::string& filename);
    ~PromptPassword();
    PromptPassword(const PromptPassword&) = delete;
    PromptPassword(PromptPassword&&) = delete;
    PromptPassword& operator=(const PromptPassword&) = delete;
    PromptPassword& operator=(PromptPassword&&) = delete;

    void run();

    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory() const {
        return _cryptoFactory;
    }
};

#endif  // _PROMPTPASSWORD_H
