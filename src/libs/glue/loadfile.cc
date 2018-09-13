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

#include "globals.h"
#include "loadfile.h"
#include "utils.hh"

//
// Private
//

void LoadFile::apoptosis_handler(YACURS::Event& e) {
    assert(e == YAPET::EVT_APOPTOSIS);

    if (typeid(e) == typeid(YACURS::EventEx<PromptPassword*>) &&
        promptpassword) {
        YACURS::EventEx<PromptPassword*>& evt =
            dynamic_cast<YACURS::EventEx<PromptPassword*>&>(e);

        if (evt.data() == promptpassword) {
            auto cryptoFactory{promptpassword->cryptoFactory()};

            if (cryptoFactory && !_selectedFilename.empty()) {
                mainwindow.load_password_file(_selectedFilename, cryptoFactory,
                                              false);
            }

            YACURS::EventQueue::submit(
                YACURS::EventEx<LoadFile*>(YAPET::EVT_APOPTOSIS, this));

            yapet::deleteAndZero(&promptpassword);
            evt.stop(true);
        }
        return;
    }
}

void LoadFile::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (fileloaddialog && evt.data() == fileloaddialog) {
        if (fileloaddialog->dialog_state() == YACURS::DIALOG_OK) {
            _selectedFilename = fileloaddialog->filepath();
            promptpassword = new PromptPassword(_selectedFilename);
            promptpassword->run();
        } else {
            // User pressed Cancel
            YACURS::EventQueue::submit(
                YACURS::EventEx<LoadFile*>(YAPET::EVT_APOPTOSIS, this));
        }

        yapet::deleteAndZero(&fileloaddialog);
        return;
    }

    if (confirmsave && evt.data() == confirmsave) {
        switch (confirmsave->dialog_state()) {
            case YACURS::DIALOG_YES:
                if (mainwindow.save_records())
                    run();
                else
                    YACURS::EventQueue::submit(
                        YACURS::EventEx<LoadFile*>(YAPET::EVT_APOPTOSIS, this));
                break;
            case YACURS::DIALOG_NO:
                ignore_unsaved_file = true;
                run();
                break;
            case YACURS::DIALOG_CANCEL:
                YACURS::EventQueue::submit(
                    YACURS::EventEx<LoadFile*>(YAPET::EVT_APOPTOSIS, this));
                break;
            default:
                throw std::runtime_error{
                    _("Unexpected dialog state for confirmsave dialog")};
                break;
        }

        yapet::deleteAndZero(&confirmsave);
        return;
    }
}

//
// Public
//

LoadFile::LoadFile(MainWindow& mw)
    : mainwindow{mw},
      promptpassword{nullptr},
      fileloaddialog{nullptr},
      confirmsave{nullptr},
      _selectedFilename{},
      _currentLoadedFile{mw.currentFilename()},
      ignore_unsaved_file{false} {
    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<LoadFile>(
        YACURS::EVT_WINDOW_CLOSE, this, &LoadFile::window_close_handler));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<LoadFile>(
        YAPET::EVT_APOPTOSIS, this, &LoadFile::apoptosis_handler));
}

LoadFile::~LoadFile() {
    if (promptpassword) delete promptpassword;
    if (fileloaddialog) delete fileloaddialog;
    if (confirmsave) delete confirmsave;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<LoadFile>(
            YACURS::EVT_WINDOW_CLOSE, this, &LoadFile::window_close_handler));

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<LoadFile>(YAPET::EVT_APOPTOSIS, this,
                                                &LoadFile::apoptosis_handler));
}

void LoadFile::run() {
    if (!ignore_unsaved_file && YAPET::Globals::records_changed) {
        confirmsave = new YACURS::MessageBox2(
            _("Unsaved Changes"), _currentLoadedFile,
            _("has unsaved changes. Do you want to save?"),
            YACURS::YESNOCANCEL);
        confirmsave->show();
    } else {
        // FileLoadDialog uses chdir
        fileloaddialog = new YACURS::FileLoadDialog(std::string(), true);
        fileloaddialog->show();
    }
}
