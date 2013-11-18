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

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif // TIME_WITH_SYS_TIME

#include <cstring>
#include <cerrno>

#ifdef ENABLE_PWGEN
# include "pwgendialog.h"
#endif
#include "cfg.h"
#include "mainwindow.h"
#include "globals.h"
#include "loadfile.h"
#include "createfile.h"


// QUIT
class HotKeyQ : public YACURS::HotKey {
    public:
	HotKeyQ() : HotKey('Q') {}
	HotKeyQ(const HotKeyQ& hkq): HotKey(hkq) {}
	void action() {
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	}

	HotKey* clone() const {
	    return new HotKeyQ(*this);
	}
};

class HotKeyq : public YACURS::HotKey {
    public:
	HotKeyq() : HotKey('q') {}
	HotKeyq(const HotKeyq& hkq): HotKey(hkq) {}
	void action() {
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	}

	HotKey* clone() const {
	    return new HotKeyq(*this);
	}
};

// HELP
class HotKeyH : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyH(MainWindow* p) : HotKey('H'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyH(const HotKeyH& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_help();
	}

	HotKey* clone() const {
	    return new HotKeyH(*this);
	}
};

class HotKeyh : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyh(MainWindow* p) : HotKey('h'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyh(const HotKeyh& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_help();
	}

	HotKey* clone() const {
	    return new HotKeyh(*this);
	}
};

// INFO
class HotKeyI : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyI(MainWindow* p) : HotKey('I'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyI(const HotKeyI& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_info();
	}

	HotKey* clone() const {
	    return new HotKeyI(*this);
	}
};

class HotKeyi : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyi(MainWindow* p) : HotKey('i'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyi(const HotKeyi& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_info();
	}

	HotKey* clone() const {
	    return new HotKeyi(*this);
	}
};

// READ FILE
class HotKeyR : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyR(MainWindow* p) : HotKey('R'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyR(const HotKeyR& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // LoadFile does apoptosis
	    LoadFile* loadfile = new LoadFile(ptr);
	    loadfile->run();
	}

	HotKey* clone() const {
	    return new HotKeyR(*this);
	}

};

class HotKeyr : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyr(MainWindow* p) : HotKey('r'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyr(const HotKeyr& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // LoadFile does apoptosis
	    LoadFile* loadfile = new LoadFile(ptr);
	    loadfile->run();
	}

	HotKey* clone() const {
	    return new HotKeyr(*this);
	}

};

// CREATE FILE
class HotKeyE : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyE(MainWindow* p) : HotKey('E'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyE(const HotKeyE& hkc) : HotKey(hkc), ptr(hkc.ptr) {}

	void action() {
	    // CreateFile does apoptosis
	    CreateFile* createfile = new CreateFile(ptr);
	    createfile->run();
	}

	HotKey* clone() const {
	    return new HotKeyE(*this);
	}

};

class HotKeye : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeye(MainWindow* p) : HotKey('e'), ptr(p) {
	    assert(p!=0);
	}
	HotKeye(const HotKeye& hkc) : HotKey(hkc), ptr(hkc.ptr) {}

	void action() {
	    // CreateFile does apoptosis
	    CreateFile* createfile = new CreateFile(ptr);
	    createfile->run();
	}

	HotKey* clone() const {
	    return new HotKeye(*this);
	}

};

// SAVE FILE
class HotKeyS : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyS(MainWindow* p) : HotKey('S'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyS(const HotKeyS& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->save_records();
	}

	HotKey* clone() const {
	    return new HotKeyS(*this);
	}

};

class HotKeys : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeys(MainWindow* p) : HotKey('s'), ptr(p) {
	    assert(p!=0);
	}
	HotKeys(const HotKeys& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->save_records();
	}

	HotKey* clone() const {
	    return new HotKeys(*this);
	}

};

// ADD RECORD
class HotKeyA : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyA(MainWindow* p) : HotKey('A'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyA(const HotKeyA& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_password_record(false);
	}

	HotKey* clone() const {
	    return new HotKeyA(*this);
	}

};

class HotKeya : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeya(MainWindow* p) : HotKey('a'), ptr(p) {
	    assert(p!=0);
	}
	HotKeya(const HotKeya& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_password_record(false);
	}

	HotKey* clone() const {
	    return new HotKeya(*this);
	}

};

// SORT ORDER
class HotKeyO : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyO(MainWindow* p) : HotKey('O'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyO(const HotKeyO& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->sort_asc(!ptr->sort_asc());
	}

	HotKey* clone() const {
	    return new HotKeyO(*this);
	}

};

class HotKeyo : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyo(MainWindow* p) : HotKey('o'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyo(const HotKeyo& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->sort_asc(!ptr->sort_asc());
	}

	HotKey* clone() const {
	    return new HotKeyo(*this);
	}

};

//
// Private
//

void
MainWindow::apoptosis_handler(YACURS::Event& e) {
    assert(e == YAPET::EVT_APOPTOSIS);

    if (typeid(e) == typeid(YACURS::EventEx<LoadFile*>)) {
	YACURS::EventEx<LoadFile*>& evt =
	    dynamic_cast<YACURS::EventEx<LoadFile*>&>(e);

	delete evt.data();
	evt.stop(true);
	return;
    }

    if (typeid(e) == typeid(YACURS::EventEx<CreateFile*>)) {
	YACURS::EventEx<CreateFile*>& evt =
	    dynamic_cast<YACURS::EventEx<CreateFile*>&>(e);

	delete evt.data();
	evt.stop(true);
	return;
    }
}

void
MainWindow::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (helpdialog!=0 && evt.data()==helpdialog) {
	delete helpdialog;
	helpdialog=0;
	return;
    }

    if (infodialog!=0 && evt.data()==infodialog) {
	delete infodialog;
	infodialog=0;
	return;
    }

    if (passwordrecord != 0 && evt.data() == passwordrecord) {
	if (passwordrecord->dialog_state() == YACURS::Dialog::DIALOG_OK) {
	    if (passwordrecord->changed()) {
		if (passwordrecord->newrecord()) {
		    recordlist->add(*passwordrecord->getEncEntry());
		    YACURS::Curses::statusbar()->set(_("Added new Record"));
		} else {
		    assert(record_index!= -1);

		    // select the record that has been selected when
		    // the dialog opened. Need in case a screen resize
		    // changed the selection index.
		    recordlist->high_light(record_index);
		    recordlist->selected(*passwordrecord->getEncEntry());
		    
		    YACURS::Curses::statusbar()->set(_("Updated Record"));
		}
		YAPET::Globals::records_changed=true;
	    } 
	}

		    
	// Reset the record index
	record_index=-1;

	delete passwordrecord;
	passwordrecord=0;
	return;
    }

    if (errormsgdialog != 0 && evt.data() == errormsgdialog) {
	delete errormsgdialog;
	errormsgdialog=0;
	return;
    }
}

void
MainWindow::listbox_enter_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_LISTBOX_ENTER);
    
    if (typeid(e) == typeid(YACURS::EventEx<YACURS::ListBox<YAPET::PartDec>*>)) {
	YACURS::EventEx<YACURS::ListBox<YAPET::PartDec>*>& evt =
	    dynamic_cast<YACURS::EventEx<YACURS::ListBox<YAPET::PartDec>*>&>(e);
	if (evt.data() != recordlist) return;

	assert(passwordrecord==0);
	show_password_record(true);
	return;
    }
}

//
// Protected
//

//
// Public
//
MainWindow::MainWindow(): Window(YACURS::Margin(1, 0, 1,
						0)) ,
			  recordlist(new YACURS::ListBox<YAPET::PartDec>()),
			  helpdialog(0),
			  passwordrecord(0),
			  errormsgdialog(0),
			  record_index(-1) {
    Window::widget(recordlist);
    frame(false);

    recordlist->sort_order(YACURS::ASCENDING);

    add_hotkey(HotKeyQ() );
    add_hotkey(HotKeyq() );

    add_hotkey(HotKeyH(this) );
    add_hotkey(HotKeyh(this) );

    add_hotkey(HotKeyR(this));
    add_hotkey(HotKeyr(this));

    add_hotkey(HotKeyS(this));
    add_hotkey(HotKeys(this));

    add_hotkey(HotKeyE(this));
    add_hotkey(HotKeye(this));

    add_hotkey(HotKeyA(this));
    add_hotkey(HotKeya(this));

    add_hotkey(HotKeyO(this));
    add_hotkey(HotKeyo(this));

    add_hotkey(HotKeyI(this));
    add_hotkey(HotKeyi(this));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YACURS::
						  EVT_WINDOW_CLOSE,
						  this,
						  &MainWindow::
						  window_close_handler) );

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YAPET::EVT_APOPTOSIS,
						  this,
						  &MainWindow::
						  apoptosis_handler) );

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YACURS::EVT_LISTBOX_ENTER,
						  this,
						  &MainWindow::
						  listbox_enter_handler) );
}

MainWindow::~MainWindow() {
    assert(recordlist!=0);
    delete recordlist;

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
					 MainWindow>(
                                                     YACURS::EVT_WINDOW_CLOSE,
                                                     this,
                                                     &MainWindow::
                                                     window_close_handler) );

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YAPET::EVT_APOPTOSIS,
						  this,
						  &MainWindow::
						  apoptosis_handler) );

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YACURS::EVT_LISTBOX_ENTER,
						  this,
						  &MainWindow::
						  listbox_enter_handler) );
}

void
MainWindow::load_password_file(YAPET::File* file, YAPET::Key* key) {
    if (YAPET::Globals::key)
	delete YAPET::Globals::key;
    YAPET::Globals::key = key;

    if (YAPET::Globals::file)
	delete YAPET::Globals::file;
    YAPET::Globals::file = file;

    YAPET::Globals::records_changed = false;

    recordlist->clear();

    try {
	recordlist->set(YAPET::Globals::file->read(*YAPET::Globals::key));
	std::string msg(_("Opened file: "));
	YACURS::Curses::statusbar()->set(msg +
					      YAPET::Globals::file->getFilename());
    } catch (std::exception& e) {
	delete YAPET::Globals::key;
	YAPET::Globals::key = 0;

	delete YAPET::Globals::file;
	YAPET::Globals::file = 0;

	recordlist->clear();

	assert(errormsgdialog!=0);

	errormsgdialog = new YACURS::MessageBox2(_("Error"),
						 _("Error while reading file:"),
						 e.what(),
						 YACURS::Dialog::OK_ONLY);
	errormsgdialog->show();
    }
}
	

void
MainWindow::show_password_record(bool selected) {
    assert(passwordrecord==0);
 
    if (YAPET::Globals::key==0 ||
	YAPET::Globals::file==0)
	return;

    if (selected) {
	assert(record_index==-1);
	record_index = recordlist->selected_index();
	passwordrecord=new PasswordRecord(YAPET::Globals::key, &(recordlist->selected())); 
    } else {
	passwordrecord=new PasswordRecord(YAPET::Globals::key);
    }

    passwordrecord->show();

}

void
MainWindow::save_records() {
    if (YAPET::Globals::file == 0) {
	// Do nothing and return
	return;
    }

    try {
	YAPET::Globals::file->save(recordlist->list());
	std::string msg(_("Saved file: "));
	YACURS::Curses::statusbar()->set(msg +
					  YAPET::Globals::file->getFilename());
	YAPET::Globals::records_changed=false;
    } catch (std::exception& e) {
	assert(errormsgdialog!=0);

	errormsgdialog = new YACURS::MessageBox2(_("Error"),
						 _("Error while saving file:"),
						 e.what(),
						 YACURS::Dialog::OK_ONLY);
	errormsgdialog->show();
    }
}

void
MainWindow::show_help() {
    assert(helpdialog==0);

    helpdialog=new HelpDialog;
    helpdialog->show();
}

void
MainWindow::show_info() {
    assert(infodialog==0);

    infodialog=new InfoDialog(recordlist->list().size());
    infodialog->show();
}

void
MainWindow::sort_asc(bool f) {
    if (f) {
	recordlist->sort_order(YACURS::ASCENDING);
	YACURS::Curses::statusbar()->set(_("Sort Order Ascending"));
    } else {
	recordlist->sort_order(YACURS::DESCENDING);
	YACURS::Curses::statusbar()->set(_("Sort Order Descending"));
    }
    
}

bool
MainWindow::sort_asc() const {
    return recordlist->sort_order() == YACURS::ASCENDING;
}
