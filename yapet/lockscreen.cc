// $Id$
//
// Copyright (C) 2010  Rafael Ostertag
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

#ifdef HAVE_CONFIG
# include <config.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include <file.h>
#include <structs.h>
#include <partdec.h>
#include <crypt.h>
#include <misc.h>

#include "../intl.h"
#include "colors.h"
#include "messagebox.h"
#include "dialogbox.h"
#include "passworddialog.h"
#include "cfg.h"
#include "lockscreen.h"

LockScreen::LockScreen(const YAPET::Key* k, const YAPET::File* f, bool daq) :
    lockwin (NULL),
    do_quit (false),
    dont_allow_quit (daq),
    resize_due (false),
    key (k),
    file (f) {
    assert (f->getFilename().length() > 0);
    assert (file->getFilename().length() > 0);
    }

LockScreen::~LockScreen() {
    assert (lockwin == NULL);
}

void
LockScreen::run() throw(YAPET::UI::UIException) {
    if (key == NULL) return;

    int ch;

    while (true) {
        lockwin = newwin (0, 0, 0, 0);

        if (lockwin == NULL)
            throw YAPET::UI::UIException (_ ("Error creating lock window") );

        int retval = werase (lockwin);

        if (retval == ERR) {
            delwin (lockwin);
            throw YAPET::UI::UIException (_ ("Error erasing lock window") );
        }

        retval = wrefresh (lockwin);

        if (retval == ERR) {
            delwin (lockwin);
            throw YAPET::UI::UIException (_ ("Error refreshing lock window") );
        }

        std::string locked_title (_ ("YAPET -- Locked --") );
        setTerminalTitle (locked_title);
        ch = wgetch (lockwin);
#ifdef HAVE_WRESIZE

        if (ch == KEY_RESIZE) {
            delwin (lockwin);
	    // We do not resize all windows, this leads to flicker and might
	    // show sensitive information. Instead we set a flag indicating
	    // that a resize is required.
	    resize_due = true;
	    //            YAPET::UI::BaseWindow::resizeAll();
            continue;
        }

#endif
        PasswordDialog* pwdia = NULL;
        YAPET::Key* testkey = NULL;

        try {
	    // Flush pending input
	    flushinp();

	    bool show_quit = dont_allow_quit ? false : true;
	    // In case the user does not want to show the quit button
	    show_quit = YAPET::CONFIG::config.getAllowLockQuit() ? show_quit : false;

            pwdia = new PasswordDialog (EXISTING_PW,
					file->getFilename(),
					YAPET::CONFIG::config.getPwInputTimeout(),
					show_quit);
            pwdia->run();
            testkey = pwdia->getKey();
	    do_quit = pwdia->wantsQuit();
            delete pwdia;
        } catch (YAPET::UI::UIException&) {
            if (pwdia != NULL)
                delete pwdia;

            if (testkey != NULL)
                delete testkey;

            delwin (lockwin);
            continue;
        }

        if (testkey == NULL) {
            delwin (lockwin);
	    lockwin = NULL;
	    // Do we have to quit
	    if (do_quit) {
		ungetch('q');
		return;
	    }
            continue;
        }

        if (*testkey != *key) {
            YAPET::UI::MessageBox* msgbox = NULL;

            try {
                msgbox = new YAPET::UI::MessageBox (_ ("E R R O R"), _ ("Wrong password") );
                msgbox->run();
                delete msgbox;
            } catch (YAPET::UI::UIException&) {
                if (msgbox != NULL)
                    delete msgbox;
            }
        } else {
            delete testkey;
            delwin (lockwin);
	    lockwin = NULL;
            return;
        }

        delete testkey;
        delwin (lockwin);
	lockwin = NULL;
    }
}

void
LockScreen::resize() {
        delwin (lockwin);
	lockwin = NULL;

        lockwin = newwin (0, 0, 0, 0);

        if (lockwin == NULL)
            throw YAPET::UI::UIException (_ ("Error creating lock window") );

        int retval = werase (lockwin);

        if (retval == ERR) {
            delwin (lockwin);
            throw YAPET::UI::UIException (_ ("Error erasing lock window") );
        }
}

void
LockScreen::refresh() {
    int retval = wrefresh (lockwin);
    if (retval == ERR) {
	delwin (lockwin);
	throw YAPET::UI::UIException (_ ("Error refreshing lock window") );
    }
}
