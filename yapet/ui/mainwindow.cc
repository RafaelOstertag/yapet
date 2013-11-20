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

#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif

#include <cstring>
#include <cstdlib>

#include "cfg.h"
#include "mainwindowhotkeys.h"
#include "mainwindow.h"
#include "globals.h"

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

    if (typeid(e) == typeid(YACURS::EventEx<ChangePassword*>)) {
	YACURS::EventEx<ChangePassword*>& evt =
	    dynamic_cast<YACURS::EventEx<ChangePassword*>&>(e);

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

#ifdef ENABLE_PWGEN
    if (pwgendialog!=0 && evt.data()==pwgendialog) {
	delete pwgendialog;
	pwgendialog=0;
	return;
    }
#endif

    if (passwordrecord != 0 && evt.data() == passwordrecord) {
	if (passwordrecord->dialog_state() == YACURS::DIALOG_OK) {
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

    if (confirmdelete != 0 && evt.data() == confirmdelete) {
	if (confirmdelete->dialog_state() == YACURS::DIALOG_YES) {
	    assert(record_index!= -1);

	    // select the record that has been selected when
	    // the dialog opened. Need in case a screen resize
	    // changed the selection index.
	    recordlist->high_light(record_index);
	    recordlist->delete_selected();
	    YACURS::Curses::statusbar()->set(_("Delete selected Record"));
	    YAPET::Globals::records_changed=true;
	}

	// Reset the record index
	record_index=-1;

	delete confirmdelete;
	confirmdelete=0;
	return;
    }

    if (confirmquit != 0 && evt.data() == confirmquit) {
	switch (confirmquit->dialog_state()) {
	case YACURS::DIALOG_YES:
	    assert(YAPET::Globals::key!=0);
	    assert(YAPET::Globals::file!=0);
	    // do not continue in case there were errors during save
	    if (!save_records()) break;
	case YACURS::DIALOG_NO:
	    // fall thru
	    YACURS::EventQueue::submit(YACURS::EVT_QUIT);
	    break;
	case YACURS::DIALOG_CANCEL:
	    YACURS::Curses::statusbar()->set(_("Quit aborted"));
	    break;
	default:
	    throw std::runtime_error(_("Unexpected dialog state for confirmsave dialog"));
	    break;
	}

	delete confirmquit;
	confirmquit=0;
	return;
    }

    if (searchdialog != 0 && evt.data() == searchdialog) {
	if (searchdialog->dialog_state()== YACURS::DIALOG_OK) {
	    // Make sure we don't search for an empty string.
	    if (searchdialog->input().empty()) {
		YACURS::Curses::statusbar()->set(_("Empty search string"));
	    } else {
		if (finder!=0)
		    delete finder;

		finder = new INTERNAL::Finder(searchdialog->input());

		if (!recordlist->search(*finder, 0, &last_search_index)) {
		    YACURS::Curses::statusbar()->set(std::string(_("No match for ")) + searchdialog->input());
		    last_search_index=0;
		}
	    }
	}

	delete searchdialog;
	searchdialog=0;
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
			  infodialog(0),
			  confirmdelete(0),
			  confirmquit(0),
			  passwordrecord(0),
			  errormsgdialog(0),
			  searchdialog(0),
#ifdef ENABLE_PWGEN
			  pwgendialog(0),
#endif
			  finder(0),
			  record_index(-1),
			  last_search_index(0) {
    Window::widget(recordlist);
    frame(false);

    recordlist->sort_order(YACURS::ASCENDING);

    add_hotkey(HotKeyQ(*this) );
    add_hotkey(HotKeyq(*this) );

    add_hotkey(HotKeyH(*this) );
    add_hotkey(HotKeyh(*this) );

    add_hotkey(HotKeyR(*this));
    add_hotkey(HotKeyr(*this));

    add_hotkey(HotKeyS(*this));
    add_hotkey(HotKeys(*this));

    add_hotkey(HotKeyE(*this));
    add_hotkey(HotKeye(*this));

    add_hotkey(HotKeyA(*this));
    add_hotkey(HotKeya(*this));

    add_hotkey(HotKeyO(*this));
    add_hotkey(HotKeyo(*this));

    add_hotkey(HotKeyI(*this));
    add_hotkey(HotKeyi(*this));

    add_hotkey(HotKeyD(*this));
    add_hotkey(HotKeyd(*this));

    add_hotkey(HotKeyC(*this));
    add_hotkey(HotKeyc(*this));

    add_hotkey(HotKeyL());
    add_hotkey(HotKeyl());

    add_hotkey(HotKeySearch(*this));

    add_hotkey(HotKeyN(*this));
    add_hotkey(HotKeyn(*this));

#ifdef ENABLE_PWGEN
    add_hotkey(HotKeyG(*this));
    add_hotkey(HotKeyg(*this));
#endif

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

    if (helpdialog) delete helpdialog;
    if (infodialog) delete infodialog;
    if (confirmdelete) delete confirmdelete;
    if (confirmquit) delete confirmquit;
    if (passwordrecord) delete passwordrecord;
    if (errormsgdialog) delete errormsgdialog;
    if (searchdialog) delete searchdialog;
#ifdef ENABLE_PWGEN
    if (pwgendialog) delete pwgendialog;
#endif
    if (finder) delete finder;

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
#ifdef HAVE_TERMINALTITLE
	std::string ttl("YAPET");
#ifdef HAVE_BASENAME
	ttl += " (";
	// basename may modify the string
	char *tmp = strdup(YAPET::Globals::file->getFilename().c_str());
	ttl += basename(tmp);

	ttl += ")";
	free(tmp);
#endif
	YACURS::Curses::set_terminal_title(ttl);
#endif
    } catch (std::exception& e) {
	delete YAPET::Globals::key;
	YAPET::Globals::key = 0;

	delete YAPET::Globals::file;
	YAPET::Globals::file = 0;

	recordlist->clear();

	assert(errormsgdialog==0);

	errormsgdialog = new YACURS::MessageBox2(_("Error"),
						 _("Error while reading file:"),
						 e.what(),
						 YACURS::OK_ONLY);
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

bool
MainWindow::save_records() {
    if (YAPET::Globals::file == 0) {
	// Do nothing and return
	return true;
    }

    try {
	YAPET::Globals::file->save(recordlist->list());
	std::string msg(_("Saved file: "));
	YACURS::Curses::statusbar()->set(msg +
					  YAPET::Globals::file->getFilename());
	YAPET::Globals::records_changed=false;
	return true;
    } catch (std::exception& e) {
	assert(errormsgdialog==0);

	errormsgdialog = new YACURS::MessageBox2(_("Error"),
						 _("Error while saving file:"),
						 e.what(),
						 YACURS::OK_ONLY);
	errormsgdialog->show();
	return false;
    }
}

void
MainWindow::change_password(YAPET::Key* nk) {
    if (YAPET::Globals::key == 0 || YAPET::Globals::file == 0)
	return;

    if (nk==0)
	throw std::invalid_argument(_("New key must not be 0"));

    try {
	YAPET::Globals::file->setNewKey(*YAPET::Globals::key,
					*nk);
	// Replace existing key
	delete YAPET::Globals::key;
	YAPET::Globals::key = nk;

	YACURS::Curses::statusbar()->set(std::string(_("Changed password on ")) + YAPET::Globals::file->getFilename());
    } catch (std::exception& e) {
	assert(errormsgdialog==0);

	errormsgdialog = new YACURS::MessageBox2(_("Error"),
						 _("Error while changing password:"),
						 e.what(),
						 YACURS::OK_ONLY);
	errormsgdialog->show();
    }
}

void
MainWindow::delete_selected() {
   assert(confirmdelete==0);

    if (YAPET::Globals::key==0 ||
	YAPET::Globals::file==0)
	return;

    assert(record_index==-1);
    record_index = recordlist->selected_index();
    confirmdelete=new YACURS::MessageBox(_("Confirm Deletion"),
				 _("Do you want to delete the selected record?"),
				 YACURS::YESNO);

    confirmdelete->show();
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

#ifdef ENABLE_PWGEN
void
MainWindow::show_pwgen() {
    assert(pwgendialog==0);

    pwgendialog=new PwGenDialog;
    pwgendialog->show();
}
#endif

void
MainWindow::quit() {
    if (YAPET::Globals::records_changed) {
	assert(confirmquit==0);
	confirmquit = new YACURS::MessageBox2(_("Confirmation"),
					      _("There are unsaved changes."),
					      _("Do you want to save changes, before leaving?"),
					      YACURS::YESNOCANCEL);
	confirmquit->show();
    } else {
	YACURS::EventQueue::submit(YACURS::EVT_QUIT);
    }
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

void
MainWindow::search_first() {
    assert(searchdialog==0);

    if (YAPET::Globals::file == 0 || YAPET::Globals::key == 0 ||
	recordlist->empty())
	return; // there is nothing to search


    searchdialog = new YACURS::InputBox(_("Search"),
					_("Enter search term"));
    searchdialog->show();
}

void
MainWindow::search_next() {
    if (YAPET::Globals::file == 0 || YAPET::Globals::key == 0 ||
	recordlist->empty())
	return; // there is nothing to search

    if (finder == 0) {
	YACURS::Curses::statusbar()->set(_("No previous search. Press '/' to search."));
	return;
    }

    while (true) {
	try {
	    if (!recordlist->search(*finder, last_search_index+1, &last_search_index)) {
		YACURS::Curses::statusbar()->set(
						 std::string(_("No more matches for ")) +
						 static_cast<const std::string&>(*finder) + std::string(_(" found"))
						 );
		last_search_index = 0;
	    } else {
		YACURS::Curses::statusbar()->set(std::string(_("Next match for ")) +
						 static_cast<const std::string&>(*finder));
	    }
	    break;
	} catch (std::out_of_range&) {
	    // reset last_search_index and restart
	    last_search_index = 0;
	}
    }
}
