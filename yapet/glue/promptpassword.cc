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

#include "../intl.h"
#include "promptpassword.h"
#include "globals.h"

#include <cassert>

//
// Private
//

void
PromptPassword::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (pwdialog!=0 && evt.data()==pwdialog) {
	if (pwdialog->dialog_state()==YACURS::DIALOG_OK) {
	    std::string pw=pwdialog->password();
	    try {
		__key = new YAPET::Key(pw.c_str());
		__file = new YAPET::File(__filename, *__key, false, YAPET::Globals::config.filesecurity);
		YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
	    } catch (YAPET::YAPETInvalidPasswordException& ex) {
		assert(pwerror==0);
		pwerror = new YACURS::MessageBox3(_("Invalid Password"),
						 _("Password for file"),
						 __filename,
						 _("is not correct. Do you want to try again?"),
						 YACURS::YESNO);
		pwerror->show();
		if (__key) delete __key;
		if (__file) delete __file;

		__key = 0;
		__file = 0;
	    } catch (std::exception& ex) {
		assert(generror==0);
		generror = new YACURS::MessageBox2(_("Error"),
						  _("Got following error"),
						  ex.what(),
						  YACURS::OK_ONLY);
		generror->show();

		if (__key) delete __key;
		if (__file) delete __file;

		__key = 0;
		__file = 0;
	    }
	} else {
	    // User pressed Cancel
	    if (__key) delete __key;
	    if (__file) delete __file;
	    
	    __key = 0;
	    __file = 0;
	    YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
	}

	delete pwdialog;
	pwdialog=0;
	return;
    }

    if (pwerror && evt.data() == pwerror) {
	if (pwerror->dialog_state() == YACURS::DIALOG_YES) {
	    run();
	} else {
	    YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
	}
	delete pwerror;
	pwerror=0;
	return;
    }

    if (generror && evt.data() == generror) {
	delete generror;
	generror = 0;
	YACURS::EventQueue::submit(YACURS::EventEx<PromptPassword*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

PromptPassword::PromptPassword(const std::string& fn): pwdialog(0),
						 pwerror(0),
						 generror(0),
						 __file(0),
						 __key(0),
						 __filename(fn) {

    YACURS::EventQueue::
	connect_event(YACURS::EventConnectorMethod1<
		      PromptPassword>(YACURS::
				      EVT_WINDOW_CLOSE,
				      this,
				      &PromptPassword::
				      window_close_handler) );

						 }

PromptPassword::~PromptPassword() {
    // YAPET Key and File must not be destroyed! They might be in use
    // somewhere else.
    if (pwdialog) delete pwdialog;
    if (pwerror) delete pwerror;
    if (generror) delete generror;

    YACURS::EventQueue::disconnect_event(
					 YACURS::EventConnectorMethod1<
					 PromptPassword>(YACURS::
							 EVT_WINDOW_CLOSE,
							 this,
							 &PromptPassword::
							 window_close_handler) );
}

void
PromptPassword::run() {
    assert(pwdialog==0);
    pwdialog = new PasswordDialog(__filename);
    pwdialog->show();
}
