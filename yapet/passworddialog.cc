// $Id$

#include "passworddialog.h"
#include "messagebox.h"

void
PasswordDialog::createWindow() throw(GPSUI::UIException) {
    if (window != NULL)
	throw GPSUI::UIException("May you consider deleting the window before reallocating");

    window = newwin(getHeight(), getWidth(), getStartY(), getStartX());
    if (window == NULL)
	throw GPSUI::UIException("Error creating password dialog");

    pwidget1 = new GPSUI::PasswordWidget(getStartX() + 1,
					 getStartY() + 3,
					 getWidth() - 2);
    if (pwtype == NEW_PW)
	pwidget2 = new GPSUI::PasswordWidget(getStartX() + 1,
					     getStartY() + 5,
					     getWidth()-2);

    okbutton = new GPSUI::Button("Ok",
				 getStartX() + 1,
				 getStartY() + getHeight() - 2);

    cancelbutton = new GPSUI::Button("Cancel",
				     getStartX() + okbutton->getLength() + 2,
				     getStartY() + getHeight() - 2);

    refresh();
}

PasswordDialog::PasswordDialog(PWTYPE pt, std::string fn)
    throw(GPSUI::UIException) : window(NULL),
			 pwidget1(NULL),
			 pwidget2(NULL),
			 okbutton(NULL),
			 cancelbutton(NULL),
			 pwtype(pt),
			 key(NULL),
			 filename(fn){
    createWindow();
			 }

PasswordDialog::~PasswordDialog() {
    delete pwidget1;
    if (pwtype == NEW_PW)
	delete pwidget2;
    delete okbutton;
    delete cancelbutton;

    delwin(window);
}

void
PasswordDialog::run() throw(GPSUI::UIException) {
    while (true) {
	pwidget1->focus();
	if (pwtype == NEW_PW)
	    pwidget2->focus();

	int ch = okbutton->focus();
	if (ch == '\n') {
	    if (pwtype == NEW_PW) {
		if (pwidget1->getText() == pwidget2->getText()) {
		    key = new GPSAFE::Key(pwidget1->getText().c_str());
		    return;
		} else {
		    GPSUI::MessageBox* errmsg = NULL;
		    try {
			errmsg = new GPSUI::MessageBox("Error", "Passwords do not match");
			errmsg->run();
			delete errmsg;
		    } catch(GPSUI::UIException&) {
			if (errmsg == NULL)
			    delete errmsg;
		    }
		    pwidget1->setText("");
		    pwidget2->setText("");
		    refresh();
		    continue;
		}
	    } else {
		key = new GPSAFE::Key(pwidget1->getText().c_str());
		pwidget1->clearText();
		return;
	    }
	}

	ch = cancelbutton->focus();
	if (ch == '\n')
	    return;
    }

}

void
PasswordDialog::resize() throw(GPSUI::UIException) {
    int retval = wclear(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error clearing window");
    retval = wrefresh(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing window");
    retval = delwin(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error deleting password dialog window");

    pwidget1->clearText();
    delete pwidget1;
    if (pwtype == NEW_PW) {
	pwidget2->clearText();
	delete pwidget2;
    }
    delete okbutton;
    delete cancelbutton;

    window = NULL;
    pwidget1 = NULL;
    pwidget2 = NULL;
    okbutton = NULL;
    cancelbutton = NULL;

    createWindow();
}	

void
PasswordDialog::refresh() throw(GPSUI::UIException) {
    GPSUI::Colors::setcolor(window, GPSUI::MESSAGEBOX);
    int retval = werase(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error clearing password dialog");

    retval = box(window, 0, 0);
    if (retval == ERR)
	throw GPSUI::UIException("Error adding box");

    retval = mymvwaddstr(window, 0, 2, "P A S S W O R D");
    if (retval == ERR)
	throw GPSUI::UIException("Error setting title");

    // The label holding the file name
    retval = mymvwaddstr(window, 2, 1, filename.c_str());
    if (retval == ERR)
	throw GPSUI::UIException("Error setting label");
    
    if (pwtype == NEW_PW) {
	retval = mymvwaddstr(window, 1, 1, "Enter new password for");
	if (retval == ERR)
	    throw GPSUI::UIException("Error setting label");
	
	retval = mymvwaddstr(window, 4, 1, "Confirm password");
	if (retval == ERR)
	    throw GPSUI::UIException("Error setting label");
    } else {
	retval = mymvwaddstr(window, 1, 1, "Enter password for");
	if (retval == ERR)
	    throw GPSUI::UIException("Error setting label");
    }	

    retval = wrefresh(window);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing password dialog");
    
    pwidget1->refresh();
    if (pwtype == NEW_PW)
	pwidget2->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}
