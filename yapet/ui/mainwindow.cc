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

    if (passwordrecord != 0 && evt.data() == passwordrecord) {
#warning "To be implemented"
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
			  errormsgdialog(0) {
    Window::widget(recordlist);
    frame(false);

    add_hotkey(HotKeyQ() );
    add_hotkey(HotKeyq() );

    add_hotkey(HotKeyH(this) );
    add_hotkey(HotKeyh(this) );

    add_hotkey(HotKeyR(this));
    add_hotkey(HotKeyr(this));

    add_hotkey(HotKeyA(this));
    add_hotkey(HotKeya(this));

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
}

void
MainWindow::show_help() {
    assert(helpdialog==0);

    helpdialog=new HelpDialog;
    helpdialog->show();
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
	YACURS::Curses::statusbar()->push_msg(msg +
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
#warning "Implement showing selected record"
    assert(passwordrecord==0);
 
    if (YAPET::Globals::key==0 ||
	YAPET::Globals::file==0)
	return;

    passwordrecord=new PasswordRecord;
    passwordrecord->show();

}

void
MainWindow::save_records() {
    assert(YAPET::Globals::file != 0);
    try {
	YAPET::Globals::file->save(recordlist->list());
	std::string msg(_("Saved file: "));
	YACURS::Curses::statusbar()->push_msg(msg +
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
