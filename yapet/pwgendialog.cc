// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
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
# include <config.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#include "../intl.h"
#include "cfg.h"
#include "messagebox.h"
#include "colors.h"
#include "pwgendialog.h"
#include "globals.h"
#include "consts.h"
#include "fileopen.h"
#include "pwgen/pwgen.h"

static const char* ckbx_strings[] = {
    "Letters",
    "Digits",
    "Punctuation",
    "Special",
    "Other",
    0
};

std::string
PWGenDialog::getNameOfRNG() const {
    switch (pwgen.getRNGUsed() ) {
        case YAPET::PWGEN::DEVRANDOM:
            return "/dev/random";
        case YAPET::PWGEN::DEVURANDOM:
            return "/dev/urandom";
        case YAPET::PWGEN::LRAND48:
            return "lrand48()";
        case YAPET::PWGEN::RAND:
            return _ ("rand() (not good)");
        case YAPET::PWGEN::NONE:
            return "none!";
    }

    assert (0);
    return "oops!";
}

int
PWGenDialog::CheckBoxOptions2Charpools (uint16_t o) const {
    int charpools = 0;
    // Refer to the ckbx_strings got see the order of the options
    charpools |= o & (1 << 0) ? YAPET::PWGEN::LETTERS : 0;
    charpools |= o & (1 << 1) ? YAPET::PWGEN::DIGITS : 0;
    charpools |= o & (1 << 2) ? YAPET::PWGEN::PUNCT : 0;
    charpools |= o & (1 << 3) ? YAPET::PWGEN::SPECIAL : 0;
    charpools |= o & (1 << 4) ? YAPET::PWGEN::OTHER : 0;
    return charpools;
}


void
PWGenDialog::initCheckBoxItems() {
    ckbxitems.clear();
    const char** tmp = ckbx_strings;

    while (*tmp != 0) {
        ckbxitems.push_back (*tmp);
        tmp++;
    }
}

void
PWGenDialog::createWindows() throw (YAPET::UI::UIException) {
    if (window != NULL)
        throw YAPET::UI::UIException (_ ("May you consider deleting the window before reallocating") );

    window = newwin (windowHeight(), windowWidth(), startY(), startX() );

    if (window == NULL)
        throw YAPET::UI::UIException (_ ("Error creating password generator window") );

    initCheckBoxItems();
    pwdisplay = new YAPET::UI::ROInWidget (startX() + 1,
                                           startY() + 2,
                                           windowWidth() - 3);
    pwleninput = new YAPET::UI::IntInWidget (startX() + 1,
            startY() + 4,
            windowWidth() - 3);
    ckbxgroup = new YAPET::UI::CheckBoxGroup (_ ("Character Pools"),
            ckbxitems,
            ckbox_options,
            startX() + 1,
            startY() + 5,
            windowWidth() - 2,
            7);
    regenbutton = new YAPET::UI::Button (_ ("Regenerate"),
                                         startX() + 2,
                                         startY() + windowHeight() - 2);
    okbutton = new YAPET::UI::Button (_ ("OK"),
                                      startX() + regenbutton->getLength() + 3,
                                      startY() + windowHeight() - 2);
    cancelbutton = new YAPET::UI::Button (_ ("Cancel"),
                                          startX() + regenbutton->getLength() + okbutton->getLength() + 4,
                                          startY() + windowHeight() - 2);
    //
    // The initial password will here be computed.
    //
    // Do not move this into refresh(). That might be tempting, but we don't
    // want to alter the password everytime the window is refreshed...
    //
    pwgen.generatePassword (pwlen);
    pwdisplay->setText (pwgen.getPassword() );
}

void
PWGenDialog::printTitles() throw (YAPET::UI::UIException) {
    int retval = mymvwaddstr (window, 0, 2, _ ("P A S S W O R D  G E N E R A T O R") );

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error printing title") );

    char tmpbuff[1024];
    snprintf (tmpbuff, 1024, _ ("Generated Password (using %s):"), getNameOfRNG().c_str() );
    retval = mymvwaddstr (window, 1, 1, tmpbuff);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error printing label") );

    retval = mymvwaddstr (window, 3,
                          1,
                          _ ("Password Length") );

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error printing label") );
}

PWGenDialog::PWGenDialog() throw (YAPET::UI::UIException) :
        window (NULL),
        ckbxgroup (NULL),
        pwdisplay (NULL),
        pwleninput (NULL),
        regenbutton (NULL),
        okbutton (NULL),
        cancelbutton (NULL),
        password (""),
        pwgen (YAPET::GLOBALS::Globals::getCharacterPools() ),
        pwlen (YAPET::GLOBALS::Globals::getPasswordLength() ),
        ckbox_options (YAPET::GLOBALS::Globals::getCharacterPools() ),
        canceled (true) {
    createWindows();
}

PWGenDialog::~PWGenDialog() {
    assert (window != NULL);
    assert (pwdisplay != NULL);
    assert (pwleninput != NULL);
    assert (ckbxgroup != NULL);
    assert (regenbutton != NULL);
    assert (okbutton != NULL);
    assert (cancelbutton != NULL);
    wclear (window);
    // To be sure we don't have any sensitive information on the screen and
    // buffers (hopefully)
    wrefresh (window);
    delwin (window);
    // Save the values used. If they are unreasonable, adjust to the bare
    // minimum.
    YAPET::GLOBALS::Globals::setCharacterPools(
					       CheckBoxOptions2Charpools(ckbxgroup->getOptions()) == 0 ?
					       1 :
					       CheckBoxOptions2Charpools(ckbxgroup->getOptions())
					       );
    YAPET::GLOBALS::Globals::setPasswordLength(
					       pwleninput->getInt() == 0 ?
					       1 :
					       pwleninput->getInt()
					       );
    // Crush the widgets
    delete ckbxgroup;
    delete pwdisplay;
    delete pwleninput;
    delete regenbutton;
    delete okbutton;
    delete cancelbutton;
    delete pwleninput;
}

void
PWGenDialog::run() throw (YAPET::UI::UIException) {
    refresh();
    int ch;

    while (true) {
        // The event handler for the check box group
        while ( (ch = pwdisplay->focus() ) != '\t') {
            switch (ch) {
#ifdef HAVE_WRESIZE
                case KEY_RESIZE:
                    YAPET::UI::BaseWindow::resizeAll();
                    break;
#endif // HAVE_WRESIZE
                case KEY_ESC:
                    canceled = true;
                    return;
                default:
                    break;
            }
        }

#ifdef HAVE_WRESIZE

        while ( (ch = pwleninput->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = pwleninput->focus();
#endif // HAVE_WRESIZE

        if (ch == KEY_ESC) {
            canceled = true;
            return;
        }

        pwlen = pwleninput->getInt() > YAPET::CONFIG::Config::getDefPWLength() ? 
	    pwleninput->setInt(YAPET::CONFIG::Config::getDefPWLength()),
	    YAPET::CONFIG::Config::getDefPWLength() :
	    pwleninput->getInt();

        // The event handler for the check box group
        while ( (ch = ckbxgroup->focus() ) != '\t') {
            switch (ch) {
#ifdef HAVE_WRESIZE
                case KEY_RESIZE:
                    YAPET::UI::BaseWindow::resizeAll();
                    break;
#endif // HAVE_WRESIZE
                case KEY_ESC:
                    canceled = true;
                    return;
                default:
                    break;
            }
        }

RESTART_GENBUTTON:
        // The regenerate button
#ifdef HAVE_WRESIZE

        while ( (ch = regenbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = okbutton->focus();
#endif // HAVE_WRESIZE

        switch (ch) {
            case KEY_ESC:
                canceled = true;
                return;
            case '\t':
                break;
            case '\n':
            case KEY_ENTER:

                if (ckbxgroup->getOptions() == 0) {
                    YAPET::UI::MessageBox* msgbox = NULL;

                    try {
                        msgbox = new YAPET::UI::MessageBox (_ ("E R R O R"), _ ("You need to select at least one Character Pool")  );
                        msgbox->run();
                        delete msgbox;
                    } catch (YAPET::UI::UIException&) {
                        if (msgbox != NULL)
                            delete msgbox;
                    }

                    refresh();
                    goto RESTART_GENBUTTON;
                }

                if (pwleninput->getInt() == 0) {
                    YAPET::UI::MessageBox* msgbox = NULL;

                    try {
                        msgbox = new YAPET::UI::MessageBox (_ ("E R R O R"), _ ("Password must have at least one character")  );
                        msgbox->run();
                        delete msgbox;
                    } catch (YAPET::UI::UIException&) {
                        if (msgbox != NULL)
                            delete msgbox;
                    }

                    refresh();
                    goto RESTART_GENBUTTON;
                }

                pwgen.setNewPool (CheckBoxOptions2Charpools (ckbxgroup->getOptions() ) );
                pwgen.generatePassword (pwlen);
                pwdisplay->clearText();
                pwdisplay->setText (pwgen.getPassword() );
                // Fall thru
            default:
                // We want to keep on the button unless the user presses a tab key
                goto RESTART_GENBUTTON;
        }

        // The ok button
#ifdef HAVE_WRESIZE

        while ( (ch = okbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = okbutton->focus();
#endif // HAVE_WRESIZE

        switch (ch) {
            case KEY_ESC:
                canceled = true;
                return;
            case '\n':
            case KEY_ENTER:
                canceled = false;

                // Store the password now
                if (pwdisplay->hasText() )
                    password = pwdisplay->getText();
                else
                    canceled = true;

                return;
        }

        // The cancel button
#ifdef HAVE_WRESIZE

        while ( (ch = cancelbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = cancelbutton->focus();
#endif // HAVE_WRESIZE

        if (ch == '\n' || ch == KEY_ENTER || ch == KEY_ESC) {
            canceled = true;
            return;
        }
    }
}

void
PWGenDialog::refresh() throw (YAPET::UI::UIException) {
    YAPET::UI::Colors::setcolor (window, YAPET::UI::MESSAGEBOX);
    int retval = box (window, 0, 0);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error drawing box") );

    printTitles();
    retval = wrefresh (window);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error refreshing window") );

    ckbxgroup->refresh();
    pwdisplay->refresh();
    pwleninput->setInt (pwlen);
    pwleninput->refresh();
    regenbutton->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}

void
PWGenDialog::resize() throw (YAPET::UI::UIException) {
    wclear (window);
    // To be sure we don't have any sensitive information on the screen and
    // buffers (hopefully)
    wrefresh (window);
    delwin (window);
    delete pwdisplay;
    delete pwleninput;
    delete ckbxgroup;
    delete regenbutton;
    delete okbutton;
    delete cancelbutton;
    window = NULL;
    pwdisplay = NULL;
    pwleninput = NULL;
    ckbxgroup = NULL;
    regenbutton = NULL;
    okbutton = NULL;
    cancelbutton = NULL;
    createWindows();
}
