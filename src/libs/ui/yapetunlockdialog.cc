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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "globals.h"
#include "intl.h"
#include "yapetunlockdialog.h"

//
// Protected
//
void YapetUnlockDialog::window_show_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_WINDOW_SHOW);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

    if (evt.data() == this) {
        assert(!_mainWindow.currentFilename().empty());
        _text2->label(_mainWindow.currentFilename());

        if (_quit) _quit->enabled(!YAPET::Globals::records_changed);
    }
}

void YapetUnlockDialog::button_press_handler(YACURS::Event& _e) {
    UnlockDialog::button_press_handler(_e);

    YACURS::EventEx<YACURS::Button*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if (_quit != nullptr && evt.data() == _quit) {
        assert(!YAPET::Globals::records_changed);
        YACURS::EventQueue::submit(YACURS::EVT_QUIT);
    }
}

//
// Public
//
YapetUnlockDialog::YapetUnlockDialog(const YACURS::Window& mainWindow)
    : UnlockDialog{_("Unlock Screen")},
      _mainWindow{dynamic_cast<const MainWindow&>(mainWindow)},
      _vpack{nullptr},
      _text1{new YACURS::DynLabel{_("Please enter password for")}},
      _text2{new YACURS::DynLabel{_("<unknown>")}},
      _text3{new YACURS::DynLabel{_("in order to unlock screen")}},
      _secret_input{new YACURS::Input<>{}},
      _quit{nullptr},
      _quit_spacer{nullptr} {
    _vpack = new YACURS::VPack;
    _vpack->always_dynamic(true);

    _text1->color(YACURS::DIALOG);
    _text2->color(YACURS::DIALOG);
    _text3->color(YACURS::DIALOG);

    _secret_input->obscure_input(true);

    _vpack->add_back(_text1);
    _vpack->add_back(_text2);
    _vpack->add_back(_text3);
    _vpack->add_back(_secret_input);

    if (YAPET::Globals::config.allow_lock_quit) {
        _quit_spacer = new YACURS::Spacer;
        add_button(_quit_spacer);
        _quit = new YACURS::Button(_("Quit"));
        add_button(_quit);
    }

    widget(_vpack);

    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<YapetUnlockDialog>(
            YACURS::EVT_WINDOW_SHOW, this,
            &YapetUnlockDialog::window_show_handler));
}

YapetUnlockDialog::~YapetUnlockDialog() {
    assert(_vpack != nullptr);
    assert(_text1 != nullptr);
    assert(_text2 != nullptr);
    assert(_text3 != nullptr);
    assert(_secret_input != nullptr);

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<YapetUnlockDialog>(
            YACURS::EVT_WINDOW_SHOW, this,
            &YapetUnlockDialog::window_show_handler));

    delete _text1;
    delete _text2;
    delete _text3;
    delete _secret_input;

    if (_quit_spacer) delete _quit_spacer;
    if (_quit) delete _quit;

    delete _vpack;
}

bool YapetUnlockDialog::unlock() {
    if (dialog_state() == YACURS::DIALOG_OK) {
        auto suppliedPassword{yapet::toSecureArray(_secret_input->input())};
        return _mainWindow.matchPasswordWithCurrent(suppliedPassword);
    }

    return false;
}

void YapetUnlockDialog::clear() { _secret_input->clear(); }
