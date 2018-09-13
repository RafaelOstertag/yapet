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
#include <cstdio>
#include <memory>
#include <typeinfo>

#include "aes256factory.hh"
#include "cfg.h"
#include "changepassword.h"
#include "cryptofactoryhelper.hh"
#include "fileerror.hh"
#include "globals.h"
#include "intl.h"
#include "utils.hh"
#include "yapeterror.hh"

//
// Private
//

void ChangePassword::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == promptoldpassword) {
        if (promptoldpassword->dialog_state() == YACURS::DIALOG_OK) {
            try {
                assert(!_oldCryptoFactory);

                auto oldPassword{
                    yapet::toSecureArray(promptoldpassword->password())};

                _oldCryptoFactory = yapet::getCryptoFactoryForFile(
                    _currentFilename, oldPassword);
                if (!_oldCryptoFactory) {
                    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
                    std::snprintf(msg,
                                  YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                                  _("File '%s' not recognized"),
                                  _currentFilename.c_str());
                    throw yapet::FileFormatError{msg};
                }

                try {
                    // Test if we can read the file with the old password
                    YAPET::File{_oldCryptoFactory, _currentFilename, false,
                                YAPET::Globals::config.filesecurity};

                    assert(promptpassword == nullptr);
                    promptpassword = new NewPasswordDialog(_currentFilename);
                    promptpassword->show();
                } catch (yapet::InvalidPasswordError& e) {
                    assert(nonmatch == nullptr);
                    nonmatch = new YACURS::MessageBox2(
                        _("Error"), _("Password does not match old password"),
                        _("Retry?"), YACURS::YESNO);
                    nonmatch->show();
                }
            } catch (std::exception& ex) {
                assert(generror == nullptr);
                generror = new YACURS::MessageBox2(_("Error"),
                                                   _("Got following error"),
                                                   ex.what(), YACURS::OK_ONLY);
                generror->show();
            }
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
        }

        // Do not put aptoptosis here, since here we can't decide
        // whether we had an exception or not. And if we had an
        // exception, generror is active and we have to wait for the
        // user to close it.

        yapet::deleteAndZero(&promptoldpassword);
    }

    if (evt.data() == nonmatch) {
        if (nonmatch->dialog_state() == YACURS::DIALOG_YES) {
            run();
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
        }

        yapet::deleteAndZero(&nonmatch);
    }

    if (evt.data() == promptpassword) {
        if (promptpassword->dialog_state() == YACURS::DIALOG_OK) {
            assert(promptpassword->match());
            assert(!_currentFilename.empty());

            try {
                assert(_oldCryptoFactory);
                auto newPassword{
                    yapet::toSecureArray(promptpassword->password())};

                std::shared_ptr<yapet::AbstractCryptoFactory> newCryptoFactory{
                    new yapet::Aes256Factory{newPassword}};

                // Try to open the current file with new password, if that
                // succeeds, the new and old password are the same and we do
                // nothing
                try {
                    auto _oldCryptoFactoryWithNewPassword{
                        _oldCryptoFactory->newFactory(newPassword)};
                    YAPET::File{_oldCryptoFactoryWithNewPassword,
                                _currentFilename, false,
                                YAPET::Globals::config.filesecurity};
                    YACURS::Curses::statusbar()->set(
                        _("Password not changed. Old and new password "
                          "identical."));
                } catch (yapet::InvalidPasswordError& e) {
                    mainwindow.change_password(newCryptoFactory);
                }

                YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(
                    YAPET::EVT_APOPTOSIS, this));
            } catch (std::exception& ex) {
                assert(generror == nullptr);
                generror = new YACURS::MessageBox2(_("Error"),
                                                   _("Got following error"),
                                                   ex.what(), YACURS::OK_ONLY);
                generror->show();
            }
        }

        // Do not put aptoptosis here, since here we can't decide
        // whether we had an exception or not. And if we had an
        // exception, generror is active and we have to wait for the
        // user to close it.

        yapet::deleteAndZero(&promptpassword);

        return;
    }

    if (evt.data() == confirmsave) {
        switch (confirmsave->dialog_state()) {
            case YACURS::DIALOG_YES:
                mainwindow.save_records();
                run();
                break;
            case YACURS::DIALOG_NO:
                YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(
                    YAPET::EVT_APOPTOSIS, this));
                break;
            default:
                throw std::runtime_error{
                    _("Unexpected dialog state for confirmsave dialog")};
                break;
        }

        yapet::deleteAndZero(&confirmsave);
        return;
    }

    if (evt.data() == generror) {
        yapet::deleteAndZero(&generror);
        YACURS::EventQueue::submit(
            YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

ChangePassword::ChangePassword(MainWindow& mw)
    : mainwindow{mw},
      promptoldpassword{nullptr},
      promptpassword{nullptr},
      nonmatch{nullptr},
      confirmsave{nullptr},
      generror{nullptr},
      _currentFilename{mw.currentFilename()},
      _oldCryptoFactory{nullptr} {
    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<ChangePassword>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &ChangePassword::window_close_handler));
}

ChangePassword::~ChangePassword() {
    if (promptoldpassword) delete promptoldpassword;
    if (promptpassword) delete promptpassword;
    if (nonmatch) delete nonmatch;
    if (confirmsave) delete confirmsave;
    if (generror) delete generror;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<ChangePassword>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &ChangePassword::window_close_handler));
}

void ChangePassword::run() {
    if (YAPET::Globals::records_changed) {
        confirmsave = new YACURS::MessageBox3(
            _("Unsaved Changes"), _("Changes in"), _currentFilename,
            _("will be saved. Do you want to continue?"), YACURS::YESNO);
        confirmsave->show();
    } else {
        promptoldpassword = new PasswordDialog(_currentFilename);
        promptoldpassword->show();
    }
}
