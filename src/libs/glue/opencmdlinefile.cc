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
#include <typeinfo>

#include "blowfishfactory.hh"
#include "file.h"
#include "globals.h"
#include "opencmdlinefile.h"

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
            assert(cryptoFactory);
            mainwindow.load_password_file(std::move(_file), cryptoFactory,
                                          false);
        }

        YACURS::EventQueue::submit(
            YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));

        delete promptpassword;
        promptpassword = 0;
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
            assert(newpassworddia == 0);
            newpassworddia = new NewPasswordDialog(_file);
            newpassworddia->show();
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));
        }
        delete createfile;
        createfile = 0;
    }

    if (evt.data() == newpassworddia) {
        if (newpassworddia->dialog_state() == YACURS::DIALOG_OK) {
            assert(newpassworddia->match());

            // Set file into configuration, so that it receives .pet
            // suffix
            YAPET::Globals::config.petfile.set(_file);

            try {
                auto password{yapet::toSecureArray(newpassworddia->password())};

                std::shared_ptr<yapet::AbstractCryptoFactory> factory{
                    new yapet::BlowfishFactory{password}};
                mainwindow.load_password_file(YAPET::Globals::config.petfile,
                                              factory, false);

                YACURS::EventQueue::submit(YACURS::EventEx<LoadFileCmdLine*>(
                    YAPET::EVT_APOPTOSIS, this));
            } catch (std::exception& ex) {
                assert(generror == 0);
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

        delete newpassworddia;
        newpassworddia = 0;

        return;
    }

    if (evt.data() == generror) {
        delete generror;
        generror = 0;
        YACURS::EventQueue::submit(
            YACURS::EventEx<LoadFileCmdLine*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

LoadFileCmdLine::LoadFileCmdLine(MainWindow& mw, const std::string& file)
    : mainwindow(mw),
      promptpassword(0),
      newpassworddia(0),
      createfile(0),
      errormsg(0),
      generror(0),
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
        assert(createfile == 0);
        createfile = new YACURS::MessageBox3(
            _("Create new File?"), _("The file"), _file,
            _("does not exist. Create?"), YACURS::YESNO);
        createfile->show();
    } else {
        promptpassword = new PromptPassword(_file);
        promptpassword->run();
    }
}
