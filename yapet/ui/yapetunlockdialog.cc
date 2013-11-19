// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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
# include "config.h"
#endif

#include "../intl.h"
#include "globals.h"
#include "yapetunlockdialog.h"

//
// Private
//

YapetUnlockDialog&
YapetUnlockDialog::operator=(const YapetUnlockDialog&) {
    throw YACURS::EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
void
YapetUnlockDialog::window_show_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_WINDOW_SHOW);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

    if (evt.data() == this) {
	assert(__text2!=0);
	assert(YAPET::Globals::file != 0);
	assert(YAPET::Globals::key != 0);
	__text2->label(YAPET::Globals::file->getFilename());

	if (__quit)
	    __quit->enabled(!YAPET::Globals::records_changed);
    }
}

void
YapetUnlockDialog::button_press_handler(YACURS::Event& _e) {
    UnlockDialog::button_press_handler(_e);

    YACURS::EventEx<YACURS::Button*>& evt = 
	dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if (__quit !=0 && evt.data() == __quit) {
	assert(!YAPET::Globals::records_changed);
	YACURS::EventQueue::submit(YACURS::EVT_QUIT);
    }
}

//
// Public
//
YapetUnlockDialog::YapetUnlockDialog(Window& mw) :
    UnlockDialog(_("Unlock Screen")),
    mainwin(dynamic_cast<MainWindow&>(mw)),
    __vpack(0),
    __text1(0),
    __text2(0),
    __text3(0),
    __secret_input(0),
    __quit(0) {

    __vpack = new YACURS::VPack;
    __vpack->always_dynamic(true);

    __text1 = new YACURS::DynLabel(_("Please enter password for"));
    __text2 = new YACURS::DynLabel(_("<unknown>"));
    __text3 = new YACURS::DynLabel(_("in order to unlock screen") );
    __text1->color(YACURS::DIALOG);
    __text2->color(YACURS::DIALOG);
    __text3->color(YACURS::DIALOG);

    __secret_input = new YACURS::Input<>;
    __secret_input->obscure_input(true);

    __vpack->add_back(__text1);
    __vpack->add_back(__text2);
    __vpack->add_back(__text3);
    __vpack->add_back(__secret_input);

    if (YAPET::Globals::config.allow_lock_quit) {
	__quit = new YACURS::Button(_("Quit"));
	__vpack->add_back(__quit);
    }

    widget(__vpack);

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<YapetUnlockDialog>(
										       YACURS::EVT_WINDOW_SHOW,
										       this,
										       &YapetUnlockDialog::window_show_handler) );
}

YapetUnlockDialog::~YapetUnlockDialog() {
    assert(__vpack != 0);
    assert(__text1 != 0);
    assert(__text2 != 0);
    assert(__text3 != 0);
    assert(__secret_input != 0);

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<YapetUnlockDialog>(
											  YACURS::EVT_WINDOW_SHOW,
											  this,
											  &YapetUnlockDialog::window_show_handler) );

    delete __vpack;
    delete __text1;
    delete __text2;
    delete __text3;
    delete __secret_input;

    if (__quit) delete __quit;
}

bool
YapetUnlockDialog::unlock() {
    assert(YAPET::Globals::file != 0);
    assert(YAPET::Globals::key != 0);

    if (dialog_state() == YACURS::DIALOG_OK) {
	YAPET::Key testkey(__secret_input->input().c_str() );
	if (testkey != *YAPET::Globals::key)
	    return false;
	else 
	    return true;
    }

    return false;
}

void
YapetUnlockDialog::clear() {
    __secret_input->clear();
}
