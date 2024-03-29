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

#include "opencmdlinefile.h"

#include <cassert>
#include <typeinfo>

#include "aes256factory.hh"
#include "cryptofactoryhelper.hh"
#include "file.hh"
#include "globals.h"
#include "utils.hh"

//
// Private
//

void LoadFileCmdLine::apoptosis_handler(YACURS::Event& e) {
    assert(e == YAPET::EVT_APOPTOSIS);

    if (typeid(e) == typeid(YACURS::EventEx<PromptPassword*>)) {
        YACURS::EventEx<PromptPassword*>& evt =
            dynamic_cast<YACURS::EventEx<PromptPassword*>&>(e);

        if (evt.data() == promptpassword) {
            auto cryptoFactory{promptpassword->cryptoFactory()};
            if (cryptoFactory) {
                mainwindow.load_password_file(_file, cryptoFactory, false);
            }
        }

        YACURS::EventQueue::submit(
            YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));

        yapet::deleteAndZero(&promptpassword);
        evt.stop(true);
    }
    return;
}

void LoadFileCmdLine::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == createfile) {
        if (createfile->dialog_state() == YACURS::DIALOG_YES) {
            assert(newpassworddia == nullptr);
            _createNewFile = true;
            newpassworddia = new NewPasswordDialog(_file);
            newpassworddia->show();
        } else {
            _createNewFile = false;
            YACURS::EventQueue::submit(
                YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));
        }

        yapet::deleteAndZero(&createfile);
    }

    if (evt.data() == newpassworddia) {
        if (newpassworddia->dialog_state() == YACURS::DIALOG_OK) {
            assert(newpassworddia->match());

            // Set file into configuration, so that it receives .pet
            // suffix
            YAPET::Globals::config.petfile.set(_file);

            try {
                auto password{yapet::toSecureArray(newpassworddia->password())};
                auto newKeyingParameters{
                    yapet::Key256::newDefaultKeyingParameters()};

                std::shared_ptr<yapet::AbstractCryptoFactory> factory{
                    new yapet::Aes256Factory{password, newKeyingParameters}};
                mainwindow.load_password_file(YAPET::Globals::config.petfile,
                                              factory, _createNewFile);

                YACURS::EventQueue::submit(YACURS::EventEx<LoadFileCmdLine*>(
                    YAPET::EVT_APOPTOSIS, this));
            } catch (std::exception& ex) {
                assert(generror == nullptr);
                generror = new YACURS::MessageBox2(_("Error"),
                                                   _("Got following error"),
                                                   ex.what(), YACURS::OK_ONLY);
                generror->show();
            }
        } else {
            // Cancel pressed
            YACURS::EventQueue::submit(
                YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));
        }

        // Do not put aptoptosis here, since here we can't decide
        // whether we had an exception or not. And if we had an
        // exception, generror is active and we have to wait for the
        // user to close it.

        yapet::deleteAndZero(&newpassworddia);

        return;
    }

    if (evt.data() == generror) {
        yapet::deleteAndZero(&generror);

        YACURS::EventQueue::submit(
            YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

LoadFileCmdLine::LoadFileCmdLine(MainWindow& mw, const std::string& file)
    : mainwindow{mw},
      promptpassword{nullptr},
      newpassworddia{nullptr},
      createfile{nullptr},
      errormsg{nullptr},
      generror{nullptr},
      _createNewFile{false},
      _file(file) {
    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<LoadFileCmdLine>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &LoadFileCmdLine::window_close_handler));

    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<LoadFileCmdLine>(
            YAPET::EVT_APOPTOSIS, this, &LoadFileCmdLine::apoptosis_handler));
}

LoadFileCmdLine::~LoadFileCmdLine() {
    if (promptpassword) delete promptpassword;
    if (newpassworddia) delete newpassworddia;
    if (createfile) delete createfile;
    if (errormsg) delete errormsg;
    if (generror) delete generror;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<LoadFileCmdLine>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &LoadFileCmdLine::window_close_handler));

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<LoadFileCmdLine>(
            YAPET::EVT_APOPTOSIS, this, &LoadFileCmdLine::apoptosis_handler));
}

void LoadFileCmdLine::run() {
    if (::access(_file.c_str(), F_OK | R_OK) == -1) {
        assert(createfile == nullptr);
        createfile = new YACURS::MessageBox3(
            _("Create new File?"), _("The file"), _file,
            _("does not exist. Create?"), YACURS::YESNO);
        createfile->show();
    } else {
        promptpassword = new PromptPassword(_file);
        promptpassword->run();
    }
}
