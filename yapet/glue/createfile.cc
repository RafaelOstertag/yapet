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
#include "globals.h"
#include "createfile.h"

#include <typeinfo>
#include <cassert>

//
// Private
//

void
CreateFile::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == filesavedialog) {
	if (filesavedialog->dialog_state() == YACURS::DIALOG_OK) {
	    __filepath = filesavedialog->filepath();
	    promptpassword = new NewPasswordDialog(__filepath);
	    promptpassword->show();
	} else {
	    YACURS::EventQueue::submit(YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
	}

	delete filesavedialog;
	filesavedialog=0;

	return;
    }

    if (evt.data() == promptpassword) {
	if (promptpassword->dialog_state() == YACURS::DIALOG_OK) {
	    assert(promptpassword->match());
	    assert(!__filepath.empty());
	    
	    YAPET::Key* _key=0;
	    YAPET::File* _file=0;
	    try {
		// Must not be delete by CreateFile
		_key = new YAPET::Key(promptpassword->password().c_str());
		// Must not be delete by CreateFile
		_file = new YAPET::File(__filepath, *_key, true, YAPET::Globals::config.filesecurity);
		mainwindow.load_password_file(_file, _key);

		YACURS::EventQueue::submit(YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
	    } catch (std::exception& ex) {
		assert(generror==0);
		generror = new YACURS::MessageBox2(_("Error"),
						   _("Got following error"),
						   ex.what(),
						   YACURS::OK_ONLY);
		generror->show();
		
		if (_key) delete _key;
		if (_file) delete _file;
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
	    ignore_unsaved_file=true;
	    run();
	    break;
	case YACURS::DIALOG_CANCEL:
	    YACURS::EventQueue::submit(YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
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
	YACURS::EventQueue::submit(YACURS::EventEx<CreateFile*>(YAPET::EVT_APOPTOSIS, this));
    }
}

//
// Public
//

CreateFile::CreateFile(MainWindow& mw): mainwindow(mw),
					promptpassword(0),
					filesavedialog(0),
					confirmsave(0),
					generror(0),
					ignore_unsaved_file(false) {

    YACURS::EventQueue::
	connect_event(YACURS::EventConnectorMethod1<
		      CreateFile>(YACURS::
				EVT_WINDOW_CLOSE,
				this,
				&CreateFile::
				window_close_handler) );
}

CreateFile::~CreateFile() {
    if (promptpassword) delete promptpassword;
    if (filesavedialog) delete filesavedialog;
    if (confirmsave) delete confirmsave;
    if (generror) delete generror;

    YACURS::EventQueue::
	disconnect_event(YACURS::EventConnectorMethod1<
			 CreateFile>(YACURS::
				   EVT_WINDOW_CLOSE,
				   this,
				   &CreateFile::
				   window_close_handler) );
}

void
CreateFile::run() {
    if (!ignore_unsaved_file &&
	YAPET::Globals::records_changed) {
	assert(YAPET::Globals::file!=0);
	confirmsave = new YACURS::MessageBox2(_("Unsaved Changes"),
					      YAPET::Globals::file->getFilename(),
					      _("has unsaved changes. Do you want to save?"),
					      YACURS::YESNOCANCEL);
	confirmsave->show();
    } else {
	filesavedialog = new YACURS::FileSaveDialog();
	filesavedialog->show();
    }
}
