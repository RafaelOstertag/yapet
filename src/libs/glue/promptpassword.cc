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

#include "promptpassword.h"

#include <cassert>
#include <cstdio>

#include "cryptofactoryhelper.hh"
#include "file.hh"
#include "fileerror.hh"
#include "globals.h"
#include "intl.h"
#include "utils.hh"
#include "yapeterror.hh"

//
// Private
//

void PromptPassword::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (pwdialog != nullptr && evt.data() == pwdialog) {
        if (pwdialog->dialog_state() == YACURS::DIALOG_OK) {
            try {
                yapet::SecureArray password{
                    yapet::toSecureArray(pwdialog->password().c_str())};

                std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
                    yapet::getCryptoFactoryForFile(_filename, password)};

                if (!cryptoFactory) {
                    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
                    std::snprintf(
                        msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                        _("File '%s' not recognized"), _filename.c_str());
                    throw yapet::FileFormatError{msg};
                }

                // This will raise an exception if password is wrong
                YAPET::File{cryptoFactory, _filename, false,
                            YAPET::Globals::config.filesecurity};

                // We were able to open the file with the given password,
                // remember that cryptFactory
                _cryptoFactory = cryptoFactory;
                YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(
                    YAPET::EVT_APOPTOSIS, this));
            } catch (yapet::InvalidPasswordError& ex) {
                assert(pwerror == nullptr);
                pwerror = new YACURS::MessageBox3(
                    _("Invalid Password"), _("Password for file"), _filename,
                    _("is not correct. Do you want to try again?"),
                    YACURS::YESNO);
                pwerror->show();
            } catch (std::exception& ex) {
                assert(generror == nullptr);
                generror = new YACURS::MessageBox2(_("Error"),
                                                   _("Got following error"),
                                                   ex.what(), YACURS::OK_ONLY);
                generror->show();
            }
        } else {
            // User pressed Cancel

            YACURS::EventQueue::submit(
                YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
        }

        yapet::deleteAndZero(&pwdialog);
        return;
    }

    if (pwerror && evt.data() == pwerror) {
        if (pwerror->dialog_state() == YACURS::DIALOG_YES) {
            run();
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
        }
        yapet::deleteAndZero(&pwerror);
        return;
    }

    if (generror && evt.data() == generror) {
        yapet::deleteAndZero(&generror);
        YACURS::EventQueue::submit(
            YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

PromptPassword::PromptPassword(const std::string& filename)
    : pwdialog{nullptr},
      pwerror{nullptr},
      generror{nullptr},
      _filename(filename),
      _cryptoFactory{nullptr} {
    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<PromptPassword>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &PromptPassword::window_close_handler));
}

PromptPassword::~PromptPassword() {
    if (pwdialog) delete pwdialog;
    if (pwerror) delete pwerror;
    if (generror) delete generror;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<PromptPassword>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &PromptPassword::window_close_handler));
}

void PromptPassword::run() {
    assert(pwdialog == nullptr);
    pwdialog = new PasswordDialog(_filename);
    pwdialog->show();
}
