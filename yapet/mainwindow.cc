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
	    ptr->file_open();
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
	    ptr->file_open();
	}

	HotKey* clone() const {
	    return new HotKeyr(*this);
	}

};
//
// Private
//
void
MainWindow::open(std::string& fn) {

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

    if (fileopendialog!=0 && evt.data()==fileopendialog) {
	if (fileopendialog->dialog_state() == YACURS::DIALOG_OK) {
	    open(fileopendialog->filepath());
	}
	delete fileopendialog;
	fileopendialog=0;
	return;
    }

    if (passworddialog!=0 && evt.data()==passworddialog) {

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
			  fileopendialog(0),
			  passworddialog(0) {
    Window::widget(recordlist);
    frame(false);

    add_hotkey(HotKeyQ() );
    add_hotkey(HotKeyq() );

    add_hotkey(HotKeyH(this) );
    add_hotkey(HotKeyh(this) );

    add_hotkey(HotKeyR(this));
    add_hotkey(HotKeyr(this));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
				      MainWindow>(YACURS::
						  EVT_WINDOW_CLOSE,
						  this,
						  &MainWindow::
						  window_close_handler) );
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
}

void
MainWindow::show_help() {
    assert(helpdialog==0);

    helpdialog=new HelpDialog;
    helpdialog->show();
}

void
MainWindow::show_file_open() {
    assert(fileopendialog==0);

    fileopendialog=new YACURS::FileLoadDialog;
    fileopendialog->show();
}
