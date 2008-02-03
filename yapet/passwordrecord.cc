// $Id$

#include "passwordrecord.h"
#include <messagebox.h>

#include <string.h>


#include <structs.h>
#include <partdec.h>
#include <crypt.h>

void
PasswordRecord::createWindow() throw(GPSUI::UIException) {
    if (window != NULL)
	throw GPSUI::UIException("May you consider deleting the window before reallocating");

    window = newwin(getHeight(), getWidth(), getStartY(), getStartX());
    if (window == NULL)
	throw GPSUI::UIException("Error creating password entry");

    name = new GPSUI::InputWidget(getStartX() + 1,
				  getStartY() + 2,
				  getWidth() - 2,
				  GPSAFE::NAME_SIZE);

    host = new GPSUI::InputWidget(getStartX() + 1,
				  getStartY() + 4,
				  getWidth() - 2,
				  GPSAFE::HOST_SIZE);

    username = new GPSUI::InputWidget(getStartX() + 1,
				      getStartY() + 6,
				      getWidth() - 2,
				      GPSAFE::USERNAME_SIZE);

    password = new GPSUI::InputWidget(getStartX() + 1,
				      getStartY() + 8,
				      getWidth() - 2,
				      GPSAFE::PASSWORD_SIZE);

    comment = new GPSUI::InputWidget(getStartX() + 1,
				     getStartY() + 10,
				     getWidth() - 2,
				     GPSAFE::COMMENT_SIZE);

    okbutton = new GPSUI::Button("Ok",
				 getStartX() + 1,
				 getStartY() + 12);

    cancelbutton = new GPSUI::Button("Cancel",
				     getStartX() + okbutton->getLength() + 2,
				     getStartY() + 12);

    refresh();
}

PasswordRecord::PasswordRecord(GPSAFE::Key& k, GPSAFE::PartDec* pe) 
    throw(GPSUI::UIException) : window(NULL),
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
PasswordRecord::run() throw(GPSUI::UIException) {
    while (true) {
	int ch = name->focus();
	ch = host->focus();
	ch = username->focus();
	ch = password->focus();
	ch = comment->focus();

	ch = okbutton->focus();
	if (ch == '\n') {
	    if (!entryChanged()) {
		encentry = NULL;
		return;
	    }

	    GPSAFE::Record<GPSAFE::PasswordRecord> unenc_rec;
	    GPSAFE::PasswordRecord* ptr_rec = unenc_rec;
	    strncpy((char*)ptr_rec->name, name->getText().c_str(), GPSAFE::NAME_SIZE);
	    strncpy((char*)ptr_rec->host, host->getText().c_str(), GPSAFE::HOST_SIZE);
	    strncpy((char*)ptr_rec->username, username->getText().c_str(), GPSAFE::USERNAME_SIZE);
	    strncpy((char*)ptr_rec->password, password->getText().c_str(), GPSAFE::PASSWORD_SIZE);
	    strncpy((char*)ptr_rec->comment, comment->getText().c_str(), GPSAFE::COMMENT_SIZE);

	    GPSAFE::BDBuffer* enc_rec = NULL;
	    try {
		encentry = new GPSAFE::PartDec(unenc_rec, *key);
	    } catch (GPSAFE::GPSException& ex) {
		if (enc_rec != NULL)
		    delete enc_rec;
		encentry = NULL;

		GPSUI::MessageBox* msgbox = NULL;
		try {
		    msgbox = new GPSUI::MessageBox("Error", ex.what());
		    msgbox->run();
		    delete msgbox;
		} catch (GPSUI::UIException&) {
		    if (msgbox != NULL)
			delete msgbox;
		    // What should I do else, looks pretty screwed up??
		}
	    }
	    return;
	}
	ch = cancelbutton->focus();
	if (ch == '\n') {
	    encentry = NULL;
	    return;
	}
    }
}

void
PasswordRecord::resize() throw(GPSUI::UIException) {
    int retval = wclear(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error clearing window");
    retval = wrefresh(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing window");
    retval = delwin(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error deleting password entry window");

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
PasswordRecord::refresh() throw(GPSUI::UIException) {
    GPSUI::Colors::setcolor(window, GPSUI::MESSAGEBOX);

    int retval = wclear(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error clearing window");

    retval = box(window, 0, 0);
    if (retval == ERR)
	throw GPSUI::UIException("Error adding box");

    retval = mymvwaddstr(window, 0, 2, "P A S S W O R D  R E C O R D");
    if (retval == ERR)
	throw GPSUI::UIException("Error setting label");

    retval = mymvwaddstr(window, 1, 1, "Name");
    if (retval == ERR)
	throw GPSUI::UIException("Error adding label");

    retval = mymvwaddstr(window, 3, 1, "Host");
    if (retval == ERR)
	throw GPSUI::UIException("Error adding label");

    retval = mymvwaddstr(window, 5, 1, "Username");
    if (retval == ERR)
	throw GPSUI::UIException("Error adding label");

    retval = mymvwaddstr(window, 7, 1, "Password");
    if (retval == ERR)
	throw GPSUI::UIException("Error adding label");

    retval = mymvwaddstr(window, 9, 1, "Comment");
    if (retval == ERR)
	throw GPSUI::UIException("Error adding label");

    retval = wrefresh(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing window");


    if (encentry != NULL) {
	GPSAFE::Record<GPSAFE::PasswordRecord>* dec_rec = NULL;
	try {
	    GPSAFE::Crypt crypt(*key);
	    dec_rec = crypt.decrypt<GPSAFE::PasswordRecord>(encentry->getEncRecord());
	    GPSAFE::PasswordRecord* ptr_rec = *dec_rec;
	    name->setText((char*)ptr_rec->name);
	    host->setText((char*)ptr_rec->host);
	    username->setText((char*)ptr_rec->username);
	    password->setText((char*)ptr_rec->password);
	    comment->setText((char*)ptr_rec->comment);
	    delete dec_rec;
	} catch (GPSAFE::GPSException& ex) {
	    if (dec_rec != NULL)
		delete dec_rec;
	    GPSUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new GPSUI::MessageBox("Error", ex.what());
		msgbox->run();
		delete msgbox;
	    } catch (GPSUI::UIException&) {
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
