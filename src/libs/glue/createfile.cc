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
#include <memory>
#include <typeinfo>

#include "aes256factory.hh"
#include "consts.h"
#include "createfile.h"
#include "globals.h"
#include "intl.h"
#include "utils.hh"

//
// Private
//

void CreateFile::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == filesavedialog) {
        if (filesavedialog->dialog_state() == YACURS::DIALOG_OK) {
            _filepath = filesavedialog->filepath();
            assert(promptpassword == nullptr);
            promptpassword = new NewPasswordDialog(_filepath);
            promptpassword->show();
        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
        }

        yapet::deleteAndZero(&filesavedialog);

        return;
    }

    if (evt.data() == promptpassword) {
        if (promptpassword->dialog_state() == YACURS::DIALOG_OK) {
            assert(promptpassword->match());
            assert(!_filepath.empty());

            // Set file into configuration, so that it receives .pet
            // suffix
            YAPET::Globals::config.petfile.set(_filepath);

            try {
                auto password{yapet::toSecureArray(promptpassword->password())};
                std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
                    new yapet::Aes256Factory{password}};
                mainwindow.load_password_file(YAPET::Globals::config.petfile,
                                              cryptoFactory, true);

                YACURS::EventQueue::submit(
                    YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
            } catch (std::exception& ex) {
                assert(generror == nullptr);
                generror = new YACURS::MessageBox2(_("Error"),
                                                   _("Got following error"),
                                                   ex.what(), YACURS::OK_ONLY);
                generror->show();
            }

        } else {
            YACURS::EventQueue::submit(
                YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
        }

        // Do not put aptoptosis here, since here we can't decide
        // whether we had an exception or not. And if we had an
        // exception, generror is active and we have to wait for the
        // user to close it. If we have an exception, generror handler
        // takes care

        yapet::deleteAndZero(&promptpassword);
        return;
    }

    if (evt.data() == confirmsave) {
        switch (confirmsave->dialog_state()) {
            case YACURS::DIALOG_YES:
                if (mainwindow.save_records())
                    run();
                else
                    YACURS::EventQueue::submit(YACURS::EventEx<CreateFile*>(
                        YAPET::EVT_APOPTOSIS, this));
                break;
            case YACURS::DIALOG_NO:
                ignore_unsaved_file = true;
                run();
                break;
            case YACURS::DIALOG_CANCEL:
                YACURS::EventQueue::submit(
                    YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
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
            YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

CreateFile::CreateFile(MainWindow& mw)
    : mainwindow{mw},
      promptpassword{nullptr},
      filesavedialog{nullptr},
      confirmsave{nullptr},
      generror{nullptr},
      _currentLoadedFile{mw.currentFilename()},
      _filepath{},
      ignore_unsaved_file{false} {
    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<CreateFile>(
        YACURS::EVT_WINDOW_CLOSE, this, &CreateFile::window_close_handler));
}

CreateFile::~CreateFile() {
    if (promptpassword) delete promptpassword;
    if (filesavedialog) delete filesavedialog;
    if (confirmsave) delete confirmsave;
    if (generror) delete generror;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<CreateFile>(
            YACURS::EVT_WINDOW_CLOSE, this, &CreateFile::window_close_handler));
}

void CreateFile::run() {
    if (!ignore_unsaved_file && YAPET::Globals::records_changed) {
        assert(confirmsave == nullptr);
        confirmsave = new YACURS::MessageBox2(
            _("Unsaved Changes"), _currentLoadedFile,
            _("has unsaved changes. Do you want to save?"),
            YACURS::YESNOCANCEL);
        confirmsave->show();
    } else {
        assert(filesavedialog == nullptr);
        // FileSaveDialog uses chdir().
        filesavedialog = new YACURS::FileSaveDialog(std::string(), true);
        filesavedialog->suffix(YAPET::Consts::DEFAULT_FILE_SUFFIX);
        filesavedialog->show();
    }
}
