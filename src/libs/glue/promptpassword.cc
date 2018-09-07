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

#include <cassert>

#include "blowfishfactory.hh"
#include "file.h"
#include "globals.h"
#include "intl.h"
#include "promptpassword.h"

//
// Private
//

void PromptPassword::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (pwdialog != 0 && evt.data() == pwdialog) {
        if (pwdialog->dialog_state() == YACURS::DIALOG_OK) {
            try {
                yapet::SecureArray password{
                    yapet::toSecureArray(pwdialog->password().c_str())};

                std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
                    new yapet::BlowfishFactory{password}};

                auto yapetFile{cryptoFactory->file(
                    _filename, false, YAPET::Globals::config.filesecurity)};

                // We were able to open the file with the given password,
                // remember that cryptFactory
                _cryptoFactory = cryptoFactory;
                YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(
                    YAPET::EVT_APOPTOSIS, this));
            } catch (YAPET::YAPETInvalidPasswordException& ex) {
                assert(pwerror == 0);
                pwerror = new YACURS::MessageBox3(
                    _("Invalid Password"), _("Password for file"), _filename,
                    _("is not correct. Do you want to try again?"),
                    YACURS::YESNO);
                pwerror->show();
            } catch (std::exception& ex) {
                assert(generror == 0);
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

        delete pwdialog;
        pwdialog = 0;
        return;
    }

    if (pwerror && evt.data() == pwerror) {
        if (pwerror->dialog_state() == YACURS::DIALOG_YES) {
            run();
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
        }
        delete pwerror;
        pwerror = 0;
        return;
    }

    if (generror && evt.data() == generror) {
        delete generror;
        generror = 0;
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
    assert(pwdialog == 0);
    pwdialog = new PasswordDialog(_filename);
    pwdialog->show();
}
