// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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
#include "passwordrecord.h"
#include <messagebox.h>

#include <string.h>


#include <structs.h>
#include <partdec.h>
#include <crypt.h>

void
PasswordRecord::createWindow() throw(YAPETUI::UIException) {
    if (window != NULL)
	throw YAPETUI::UIException(_("May you consider deleting the window before reallocating"));

    window = newwin(getHeight(), getWidth(), getStartY(), getStartX());
    if (window == NULL)
	throw YAPETUI::UIException(_("Error creating password entry"));

    name = new YAPETUI::InputWidget(getStartX() + 1,
				  getStartY() + 2,
				  getWidth() - 2,
				  YAPET::NAME_SIZE);

    host = new YAPETUI::InputWidget(getStartX() + 1,
				  getStartY() + 4,
				  getWidth() - 2,
				  YAPET::HOST_SIZE);

    username = new YAPETUI::InputWidget(getStartX() + 1,
				      getStartY() + 6,
				      getWidth() - 2,
				      YAPET::USERNAME_SIZE);

    password = new YAPETUI::InputWidget(getStartX() + 1,
				      getStartY() + 8,
				      getWidth() - 2,
				      YAPET::PASSWORD_SIZE);

    comment = new YAPETUI::InputWidget(getStartX() + 1,
				     getStartY() + 10,
				     getWidth() - 2,
				     YAPET::COMMENT_SIZE);

    okbutton = new YAPETUI::Button(_("OK"),
				 getStartX() + 1,
				 getStartY() + 12);

    cancelbutton = new YAPETUI::Button(_("Cancel"),
				     getStartX() + okbutton->getLength() + 2,
				     getStartY() + 12);

    refresh();
}

PasswordRecord::PasswordRecord(YAPET::Key& k, YAPET::PartDec* pe)
    throw(YAPETUI::UIException) : window(NULL),
			 name(NULL),
			 host(NULL),
			 username(NULL),
			 password(NULL),
			 comment(NULL),
			 okbutton(NULL),
			 cancelbutton(NULL),
			 key(&k),
			 encentry(pe) {
    createWindow();
			 }

PasswordRecord::~PasswordRecord() {
	wclear(window);
	delwin(window);
	delete name;
	delete host;
	delete username;
	delete password;
	delete comment;
	delete okbutton;
	delete cancelbutton;
}

void
PasswordRecord::run() throw(YAPETUI::UIException) {
    while (true) {
	int ch = 0;
#ifdef HAVE_WRESIZE
	while ( (ch = name->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = name->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}

#ifdef HAVE_WRESIZE
	while ( (ch = host->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = host->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}


#ifdef HAVE_WRESIZE
	while ( (ch = username->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = username->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}


#ifdef HAVE_WRESIZE
	while ( (ch = password->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = password->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}


#ifdef HAVE_WRESIZE
	while ( (ch = comment->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = comment->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}


#ifdef HAVE_WRESIZE
	while ( (ch = okbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = okbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}

	if (ch == '\n') {
	    if (!entryChanged()) {
		encentry = NULL;
		return;
	    }

	    YAPET::Record<YAPET::PasswordRecord> unenc_rec;
	    YAPET::PasswordRecord* ptr_rec = unenc_rec;
	    strncpy((char*)ptr_rec->name, name->getText().c_str(), YAPET::NAME_SIZE);
	    strncpy((char*)ptr_rec->host, host->getText().c_str(), YAPET::HOST_SIZE);
	    strncpy((char*)ptr_rec->username, username->getText().c_str(), YAPET::USERNAME_SIZE);
	    strncpy((char*)ptr_rec->password, password->getText().c_str(), YAPET::PASSWORD_SIZE);
	    strncpy((char*)ptr_rec->comment, comment->getText().c_str(), YAPET::COMMENT_SIZE);

	    YAPET::BDBuffer* enc_rec = NULL;
	    try {
		encentry = new YAPET::PartDec(unenc_rec, *key);
	    } catch (YAPET::YAPETException& ex) {
		if (enc_rec != NULL)
		    delete enc_rec;
		encentry = NULL;

		YAPETUI::MessageBox* msgbox = NULL;
		try {
		    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
		    msgbox->run();
		    delete msgbox;
		} catch (YAPETUI::UIException&) {
		    if (msgbox != NULL)
			delete msgbox;
		    // What should I do else, looks pretty screwed up??
		}
	    }
	    return;
	}

#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n' || ch == KEY_ESC) {
	    encentry = NULL;
	    return;
	}
    }
}

void
PasswordRecord::resize() throw(YAPETUI::UIException) {
    int retval = delwin(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error deleting password entry window"));

    delete name;
    delete host;
    delete username;
    delete password;
    delete comment;
    delete okbutton;
    delete cancelbutton;

    window = NULL;
    name = NULL;
    username = NULL;
    password = NULL;
    comment = NULL;
    okbutton = NULL;
    cancelbutton = NULL;

    createWindow();
}

void
PasswordRecord::refresh() throw(YAPETUI::UIException) {
    YAPETUI::Colors::setcolor(window, YAPETUI::MESSAGEBOX);

    int retval = wclear(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error clearing window"));

    retval = box(window, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding box"));

    retval = mymvwaddstr(window, 0, 2, _("P A S S W O R D  R E C O R D"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error setting label"));

    retval = mymvwaddstr(window, 1, 1, _("Name"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding label"));

    retval = mymvwaddstr(window, 3, 1, _("Host"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding label"));

    retval = mymvwaddstr(window, 5, 1, _("Username"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding label"));

    retval = mymvwaddstr(window, 7, 1, _("Password"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding label"));

    retval = mymvwaddstr(window, 9, 1, _("Comment"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding label"));

    retval = wrefresh(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error refreshing window"));


    if (encentry != NULL) {
	YAPET::Record<YAPET::PasswordRecord>* dec_rec = NULL;
	try {
	    YAPET::Crypt crypt(*key);
	    dec_rec = crypt.decrypt<YAPET::PasswordRecord>(encentry->getEncRecord());
	    YAPET::PasswordRecord* ptr_rec = *dec_rec;
	    name->setText((char*)ptr_rec->name);
	    host->setText((char*)ptr_rec->host);
	    username->setText((char*)ptr_rec->username);
	    password->setText((char*)ptr_rec->password);
	    comment->setText((char*)ptr_rec->comment);
	    delete dec_rec;
	} catch (YAPET::YAPETException& ex) {
	    if (dec_rec != NULL)
		delete dec_rec;
	    YAPETUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
		msgbox->run();
		delete msgbox;
	    } catch (YAPETUI::UIException&) {
		if (msgbox != NULL)
		    delete msgbox;
		// What should I do else, looks pretty screwed up??
	    }
	}
    }
    name->refresh();
    host->refresh();
    username->refresh();
    password->refresh();
    comment->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}
