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

#include "intl.h"
#include "cfg.h"
#include "globals.h"
#include "changepassword.h"

#include <typeinfo>
#include <cassert>

//
// Private
//

void
ChangePassword::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == promptpassword) {
	if (promptpassword->dialog_state() == YACURS::DIALOG_OK) {
	    assert(promptpassword->match());
	    assert(!__filepath.empty());
	    assert(YAPET::Globals::file != 0);
	    assert(YAPET::Globals::key != 0);
	    
	    YAPET::Key* _key=0;
	    try {
		// Must not be delete by ChangePassword
		_key = new YAPET::Key(promptpassword->password().c_str());
		mainwindow.change_password(_key);
		YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
	    } catch (std::exception& ex) {
		assert(generror==0);
		generror = new YACURS::MessageBox2(_("Error"),
						   _("Got following error"),
						   ex.what(),
						   YACURS::OK_ONLY);
		generror->show();
		
		if (_key) delete _key;
	    }
	    
	} 
	
	// Do not put aptoptosis here, since here we can't decide
	// whether we had an exception or not. And if we had an
	// exception, generror is active and we have to wait for the
	// user to close it.
	
	
	delete promptpassword;
	promptpassword=0;
	
	return;
    }
    
    if (evt.data() == confirmsave) {
	switch (confirmsave->dialog_state()) {
	case YACURS::DIALOG_YES: 
	    mainwindow.save_records();
	    run();
	    break;
	case YACURS::DIALOG_NO:
	    YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
	    break;
	default:
	    throw std::runtime_error(_("Unexpected dialog state for confirmsave dialog"));
	    break;
	}
	
	delete confirmsave;
	confirmsave = 0;
	return;
    }
    
    if (evt.data() == generror) {
	delete generror;
	generror = 0;
	YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

ChangePassword::ChangePassword(MainWindow& mw): mainwindow(mw),
					promptpassword(0),
					confirmsave(0),
					generror(0),
					ignore_unsaved_file(false) {

    YACURS::EventQueue::
	connect_event(YACURS::EventConnectorMethod1<
		      ChangePassword>(YACURS::
				EVT_WINDOW_CLOSE,
				this,
				&ChangePassword::
				window_close_handler) );
}

ChangePassword::~ChangePassword() {
    if (promptpassword) delete promptpassword;
    if (confirmsave) delete confirmsave;
    if (generror) delete generror;

    YACURS::EventQueue::
	disconnect_event(YACURS::EventConnectorMethod1<
			 ChangePassword>(YACURS::
				   EVT_WINDOW_CLOSE,
				   this,
				   &ChangePassword::
				   window_close_handler) );
}

void
ChangePassword::run() {
    if (YAPET::Globals::file == 0 || YAPET::Globals::key == 0) {
	    YACURS::EventQueue::submit(YACURS::EventEx<ChangePassword*>(YAPET::EVT_APOPTOSIS, this));
	    return;
    }

    if (YAPET::Globals::records_changed) {
	assert(YAPET::Globals::file!=0);
	confirmsave = new YACURS::MessageBox3(_("Unsaved Changes"),
					      _("Changes in"),
					      YAPET::Globals::file->getFilename(),
					      _("will be saved. Do you want to continue?"),
					      YACURS::YESNO);
	confirmsave->show();
    } else {
	__filepath = YAPET::Globals::file->getFilename();
	promptpassword = new NewPasswordDialog(__filepath);
	promptpassword->show();
    }
}
