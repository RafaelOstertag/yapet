// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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
#include "colors.h"
#include "passworddialog.h"
#include "messagebox.h"

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#ifdef HAVE_SETJMP_H
# include <setjmp.h>
#endif

sigjmp_buf password_dialog_sig_jmp_buf;

extern "C" void __alarm_handler(int) {
    siglongjmp(password_dialog_sig_jmp_buf, 1);
}

void
PasswordDialog::createWindow() throw (YAPET::UI::UIException) {
    if (window != NULL)
	throw YAPET::UI::UIException (_ ("May you consider deleting the window before reallocating") );

    window = newwin (getHeight(), getWidth(), getStartY(), getStartX() );

    if (window == NULL)
	throw YAPET::UI::UIException (_ ("Error creating password dialog") );

    pwidget1 = new YAPET::UI::PasswordWidget (getStartX() + 1,
	    getStartY() + 3,
	    getWidth() - 2);

    if (pwtype == NEW_PW)
	pwidget2 = new YAPET::UI::PasswordWidget (getStartX() + 1,
		getStartY() + 5,
		getWidth() - 2);

    okbutton = new YAPET::UI::Button (_ ("OK"),
				      getStartX() + 1,
				      getStartY() + getHeight() - 2);
    cancelbutton = new YAPET::UI::Button (_ ("Cancel"),
					  getStartX() + okbutton->getLength() + 2,
					  getStartY() + getHeight() - 2);
    if (has_quitbutton) {
	quitbutton = new YAPET::UI::Button (_("Quit"),
					    getStartX() + 
					    okbutton->getLength() + 1 +
					    cancelbutton->getLength() + 2,
					    getStartY() + getHeight() - 2);
    }
}

PasswordDialog::PasswordDialog (PWTYPE pt, std::string fn, unsigned int tout, bool qb)
    throw (YAPET::UI::UIException) : window (NULL),
				     pwidget1 (NULL),
				     pwidget2 (NULL),
				     okbutton (NULL),
				     cancelbutton (NULL),
				     quitbutton (NULL),
				     pwtype (pt),
				     key (NULL),
				     filename (fn),
				     input_timeout (tout),
				     has_quitbutton (qb),
				     quit_pressed (false) {
    createWindow();
}

PasswordDialog::~PasswordDialog() {
    delete pwidget1;

    if (pwtype == NEW_PW)
	delete pwidget2;

    delete okbutton;
    delete cancelbutton;
    if (has_quitbutton) {
	assert(quitbutton != NULL);
	delete quitbutton;
    }
    wclear (window);
    delwin (window);
}

void
PasswordDialog::run() throw (YAPET::UI::UIException) {
    refresh();

    if (pwtype == EXISTING_PW || input_timeout > 0) {
	int retval;

	retval = sigemptyset(&my_sigset);
	if ( retval == -1 ) {
	    throw YAPET::UI::UIException("Error calling sigemptyset()");
	}
	retval = sigaddset(&my_sigset, SIGALRM);
	if ( retval == -1 ) {
	    throw YAPET::UI::UIException("Error calling sigaddset()");
	}
	retval = sigprocmask(SIG_UNBLOCK, &my_sigset, &old_sigset);
	if ( retval == -1 ) {
	    throw YAPET::UI::UIException("Error calling sigprocmask()");
	}

	my_sigaction.sa_handler=__alarm_handler;
	retval = sigfillset(&(my_sigaction.sa_mask));
	if ( retval == -1 ) {
	    throw YAPET::UI::UIException("Error calling sigfillset()");
	}
	my_sigaction.sa_flags=0;

	if ( sigsetjmp(password_dialog_sig_jmp_buf,1) == 1) {
	    sigaction(SIGALRM, &old_sigaction, NULL);
	    sigprocmask(SIG_SETMASK, &old_sigset, NULL);

	    if ( key != NULL ) {
		delete key;
		key = NULL;
	    }
	    if ( pwidget1 != NULL ) {
		pwidget1->clearText();
	    }
	    return;
	}

	retval = sigaction(SIGALRM, &my_sigaction, &old_sigaction);
	if ( retval == -1 ) {
	    alarm(0);
	    sigaction(SIGALRM, &old_sigaction, NULL);
	    sigprocmask(SIG_SETMASK, &old_sigset, NULL);
	    throw YAPET::UI::UIException("Error calling sigprocmask()");
	}
	alarm(0);
    }

    while (true) {
	int ch = 0;
	if (pwtype == EXISTING_PW || input_timeout > 0)
	    alarm(input_timeout);

#ifdef HAVE_WRESIZE

	while ( (ch = pwidget1->focus() ) == KEY_RESIZE)
	    YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
	ch = pwidget1->focus();
#endif // HAVE_WRESIZE

	if (ch == KEY_ESC)
	    goto BAILOUT;

	// Password input widget 2 only if we prompt for a new password
	if (pwtype == NEW_PW) {
#ifdef HAVE_WRESIZE

	    while ( (ch = pwidget2->focus() ) == KEY_RESIZE)
		YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
	    ch = pwidget2->focus();
#endif // HAVE_WRESIZE

	    if (ch == KEY_ESC)
		goto BAILOUT;
	}

#ifdef HAVE_WRESIZE

	while ( (ch = okbutton->focus() ) == KEY_RESIZE)
	    YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
	ch = okbutton->focus();
#endif // HAVE_WRESIZE

	if (ch == KEY_ESC)
	    goto BAILOUT;

	if (ch == '\n') {
	    if (pwtype == NEW_PW) {
		if (pwidget1->getText() == pwidget2->getText() ) {
		    key = new YAPET::Key (pwidget1->getText().c_str() );
		    goto BAILOUT;
		} else {
		    YAPET::UI::MessageBox* errmsg = NULL;

		    try {
			errmsg = new YAPET::UI::MessageBox (_ ("E R R O R"), _ ("Passwords do not match") );
			errmsg->run();
			delete errmsg;
		    } catch (YAPET::UI::UIException&) {
			if (errmsg == NULL)
			    delete errmsg;
		    }

		    pwidget1->setText ("");
		    pwidget2->setText ("");
		    refresh();
		    continue;
		}
	    } else {
		key = new YAPET::Key (pwidget1->getText().c_str() );
		pwidget1->clearText();
		goto BAILOUT;
	    }
	}

#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus() ) == KEY_RESIZE)
	    YAPET::UI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n' || ch == KEY_ENTER || ch == KEY_ESC)
	    goto BAILOUT;

	if (has_quitbutton) {
#ifdef HAVE_WRESIZE
	    while ( (ch = quitbutton->focus() ) == KEY_RESIZE)
		YAPET::UI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	    ch = quitbutton->focus();
#endif // HAVE_WRESIZE
	    if (ch == '\n' || ch == KEY_ENTER) {
		quitPressed(true);
		goto BAILOUT;
	    }
	    if (ch == KEY_ESC) {
		quitPressed(false);
		goto BAILOUT;
	    }
	}
    }

 BAILOUT:
    if (pwtype == EXISTING_PW || input_timeout > 0) {
	alarm(0);
	sigaction(SIGALRM, &old_sigaction, NULL);
	sigprocmask(SIG_SETMASK, &old_sigset, NULL);
    }
}

void
PasswordDialog::resize() throw (YAPET::UI::UIException) {
    int retval = delwin (window);

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error deleting password dialog window") );

    pwidget1->clearText();
    delete pwidget1;

    if (pwtype == NEW_PW) {
	pwidget2->clearText();
	delete pwidget2;
    }

    delete okbutton;
    delete cancelbutton;
    if (has_quitbutton) {
	delete quitbutton;
    }
    window = NULL;
    pwidget1 = NULL;
    pwidget2 = NULL;
    okbutton = NULL;
    cancelbutton = NULL;
    quitbutton = NULL;
    createWindow();
}

void
PasswordDialog::refresh() throw (YAPET::UI::UIException) {
    YAPET::UI::Colors::setcolor (window, YAPET::UI::MESSAGEBOX);
    int retval = werase (window);

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error clearing password dialog") );

    retval = box (window, 0, 0);

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error adding box") );

    retval = mymvwaddstr (window, 0, 2, _ ("P A S S W O R D") );

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error setting title") );

    // The label holding the file name
    retval = mymvwaddstr (window, 2, 1, filename.c_str() );

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error setting label") );

    if (pwtype == NEW_PW) {
	retval = mymvwaddstr (window, 1, 1, _ ("Enter new password for") );

	if (retval == ERR)
	    throw YAPET::UI::UIException (_ ("Error setting label") );

	retval = mymvwaddstr (window, 4, 1, _ ("Confirm password") );

	if (retval == ERR)
	    throw YAPET::UI::UIException (_ ("Error setting label") );
    } else {
	retval = mymvwaddstr (window, 1, 1, _ ("Enter password for") );

	if (retval == ERR)
	    throw YAPET::UI::UIException (_ ("Error setting label") );
    }

    retval = wrefresh (window);

    if (retval == ERR)
	throw YAPET::UI::UIException (_ ("Error refreshing password dialog") );

    pwidget1->refresh();

    if (pwtype == NEW_PW)
	pwidget2->refresh();

    okbutton->refresh();
    cancelbutton->refresh();
    if (has_quitbutton)
	quitbutton->refresh();
}
