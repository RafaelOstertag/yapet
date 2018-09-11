// $Id$
//
// Copyright (C) 2008-2011, 2013  Rafael Ostertag
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
        __text2->label(_mainWindow.currentFilename());

        if (__quit) __quit->enabled(!YAPET::Globals::records_changed);
    }
}

void YapetUnlockDialog::button_press_handler(YACURS::Event& _e) {
    UnlockDialog::button_press_handler(_e);

    YACURS::EventEx<YACURS::Button*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if (__quit != nullptr && evt.data() == __quit) {
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
      __vpack{nullptr},
      __text1{new YACURS::DynLabel{_("Please enter password for")}},
      __text2{new YACURS::DynLabel{_("<unknown>")}},
      __text3{new YACURS::DynLabel{_("in order to unlock screen")}},
      __secret_input{new YACURS::Input<>{}},
      __quit{nullptr},
      __quit_spacer{nullptr} {
    __vpack = new YACURS::VPack;
    __vpack->always_dynamic(true);

    __text1->color(YACURS::DIALOG);
    __text2->color(YACURS::DIALOG);
    __text3->color(YACURS::DIALOG);

    __secret_input->obscure_input(true);

    __vpack->add_back(__text1);
    __vpack->add_back(__text2);
    __vpack->add_back(__text3);
    __vpack->add_back(__secret_input);

    if (YAPET::Globals::config.allow_lock_quit) {
        __quit_spacer = new YACURS::Spacer;
        add_button(__quit_spacer);
        __quit = new YACURS::Button(_("Quit"));
        add_button(__quit);
    }

    widget(__vpack);

    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<YapetUnlockDialog>(
            YACURS::EVT_WINDOW_SHOW, this,
            &YapetUnlockDialog::window_show_handler));
}

YapetUnlockDialog::~YapetUnlockDialog() {
    assert(__vpack != nullptr);
    assert(__text1 != nullptr);
    assert(__text2 != nullptr);
    assert(__text3 != nullptr);
    assert(__secret_input != nullptr);

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<YapetUnlockDialog>(
            YACURS::EVT_WINDOW_SHOW, this,
            &YapetUnlockDialog::window_show_handler));

    delete __text1;
    delete __text2;
    delete __text3;
    delete __secret_input;

    if (__quit_spacer) delete __quit_spacer;
    if (__quit) delete __quit;

    delete __vpack;
}

bool YapetUnlockDialog::unlock() {
    if (dialog_state() == YACURS::DIALOG_OK) {
        auto suppliedPassword{yapet::toSecureArray(__secret_input->input())};
        return _mainWindow.matchPasswordWithCurrent(suppliedPassword);
    }

    return false;
}

void YapetUnlockDialog::clear() { __secret_input->clear(); }
