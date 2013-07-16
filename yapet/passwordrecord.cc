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
# include <config.h>
#endif

#include "../intl.h"
#include "globals.h"
#include "crypt.h"
#include "passwordrecord.h"

void
PasswordRecord::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
	dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (errordialog!=0 && evt.data() == errordialog) {
	delete errordialog;
	errordialog=0;
    }
}

PasswordRecord::PasswordRecord(YAPET::PartDec* pe) :
    YACURS::Dialog(_("Password Entry")),
    vpack(new YACURS::VPack),    
    lname(new YACURS::Label(_("Name"))),
    lhost(new YACURS::Label(_("Host"))),
    lusername(new YACURS::Label(_("User Name"))),
    lpassword(new YACURS::Label(_("Password"))),
    lcomment(new YACURS::Label(_("Comment"))),
    name(new YACURS::Input<std::string>(128)),
    host(new YACURS::Input<std::string>(256)),
    username(new YACURS::Input<std::string>(256)),
    password(new YACURS::Input<std::string>(256)),
    comment(new YACURS::Input<std::string>(512)),
#ifdef ENABLE_PWGEN
    pwgenbutton(new YACURS::Button(_("Password Generator"))),
#endif
    encentry(pe),
    errordialog(0),
    __readonly(false) {

    vpack->add_back(lname);
    vpack->add_back(name);
    vpack->add_back(lhost);
    vpack->add_back(host);
    vpack->add_back(lusername);
    vpack->add_back(username);
    vpack->add_back(lpassword);
    vpack->add_back(password);
    vpack->add_back(lcomment);
    vpack->add_back(comment);
#ifdef ENABLE_PWGEN
    vpack->add_back(pwgenbutton);
#endif
    
    if (encentry != 0) {
        YAPET::Record<YAPET::PasswordRecord>* dec_rec = 0;

        try {
            YAPET::Crypt crypt(*YAPET::Globals::key);
            dec_rec = crypt.decrypt<YAPET::PasswordRecord>(
                encentry->getEncRecord() );
            YAPET::PasswordRecord* ptr_rec = *dec_rec;
            name->input(reinterpret_cast<char*>(ptr_rec->name));
            host->input(reinterpret_cast<char*>(ptr_rec->host));
            username->input(reinterpret_cast<char*>(ptr_rec->username));
            password->input(reinterpret_cast<char*>(ptr_rec->password));
            comment->input(reinterpret_cast<char*>(ptr_rec->comment));
            delete dec_rec;
        } catch (YAPET::YAPETException& ex) {
            if (dec_rec != NULL)
                delete dec_rec;

            try {
                errordialog =
                    new YACURS::MessageBox(_("Error"), ex.what() );
                errordialog->show();
            } catch (YACURS::EXCEPTIONS::BaseCurEx&) {
                if (errordialog != 0)
                    delete errordialog;

                // What should I do else, looks pretty screwed up??
            }
        }
    }
    
    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<PasswordRecord>(
										    YACURS::EVT_WINDOW_CLOSE, this,
							       &PasswordRecord::window_close_handler) );
}

PasswordRecord::~PasswordRecord() {
    delete lname;
    delete lhost;
    delete lusername;
    delete lpassword;
    delete lcomment;
    delete name;
    delete host;
    delete username;
    delete password;
    delete comment;
#ifdef ENABLE_PWGEN
    delete pwgenbutton;
#endif
    delete vpack;

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<PasswordRecord>(
										       YACURS::EVT_WINDOW_CLOSE, this,
							       &PasswordRecord::window_close_handler) );
}

#if 0
void
PasswordRecord::run() throw(YAPET::UI::UIException) {
    PasswordRecord::Alarm alrm(*this);

    while (true) {
        int ch = 0;

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

        while ( (ch = name->focus() ) ) {
            resize_as_needed();
            switch (ch) {
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
                YAPET::UI::BaseWindow::resizeAll();
                continue;
                break;

#endif // HAVE_WRESIZE
            case KEY_CTRL_E:
                setReadonly(false);
                // For getting the title right
                refresh();
                continue;
                break;
            }
            break;
        }
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

        s_name = name->getText();
        namechanged = name->isTextChanged();

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        while ( (ch = host->focus() ) ) {
            resize_as_needed();
            switch (ch) {
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
                YAPET::UI::BaseWindow::resizeAll();
                continue;
                break;

#endif // HAVE_WRESIZE
            case KEY_CTRL_E:
                setReadonly(false);
                // For getting the title right
                refresh();
                continue;
                break;
            }
            break;
        }
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        s_host = host->getText();
        hostchanged = host->isTextChanged();

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        while ( (ch = username->focus() ) ) {
            resize_as_needed();
            switch (ch) {
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
                YAPET::UI::BaseWindow::resizeAll();
                continue;
                break;

#endif // HAVE_WRESIZE
            case KEY_CTRL_E:
                setReadonly(false);
                // For getting the title right
                refresh();
                continue;
                break;
            }
            break;
        }
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

        s_username = username->getText();
        usernamechanged = username->isTextChanged();

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        while ( (ch = password->focus() ) ) {
            resize_as_needed();
            switch (ch) {
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
                YAPET::UI::BaseWindow::resizeAll();
                continue;
                break;

#endif // HAVE_WRESIZE
            case KEY_CTRL_E:
                setReadonly(false);
                // For getting the title right
                refresh();
                continue;
                break;
            }
            break;
        }
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

        s_password = password->getText();
        passwordchanged = password->isTextChanged();

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        while ( (ch = comment->focus() ) ) {
            resize_as_needed();
            switch (ch) {
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
                YAPET::UI::BaseWindow::resizeAll();
                continue;
                break;

#endif // HAVE_WRESIZE
            case KEY_CTRL_E:
                setReadonly(false);
                // For getting the title right
                refresh();
                continue;
                break;
            }
            break;
        }
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

        s_comment = comment->getText();
        commentchanged = comment->isTextChanged();

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
#ifdef HAVE_WRESIZE

        while ( (ch = okbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = okbutton->focus();
#endif // HAVE_WRESIZE
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

        if (ch == '\n') {
            if (!entryChanged() ) {
                encentry = NULL;
                goto BAILOUT;
            }

            YAPET::Record<YAPET::PasswordRecord> unenc_rec;
            YAPET::PasswordRecord* ptr_rec = unenc_rec;
            strncpy( (char*)ptr_rec->name, s_name.c_str(), YAPET::NAME_SIZE);
            strncpy( (char*)ptr_rec->host, s_host.c_str(), YAPET::HOST_SIZE);
            strncpy( (char*)ptr_rec->username,
                     s_username.c_str(), YAPET::USERNAME_SIZE);
            strncpy( (char*)ptr_rec->password,
                     s_password.c_str(), YAPET::PASSWORD_SIZE);
            strncpy( (char*)ptr_rec->comment,
                     s_comment.c_str(), YAPET::COMMENT_SIZE);
            YAPET::BDBuffer* enc_rec = NULL;

            try {
                encentry = new YAPET::PartDec(unenc_rec, *key);
            } catch (YAPET::YAPETException& ex) {
                if (enc_rec != NULL)
                    delete enc_rec;

                encentry = NULL;
                YAPET::UI::MessageBox* msgbox = NULL;

                try {
                    msgbox = new YAPET::UI::MessageBox(_(
                                                           "E R R O R"),
                                                       ex.what() );
                    msgbox->run();
                    delete msgbox;
                } catch (YAPET::UI::UIException&) {
                    if (msgbox != NULL)
                        delete msgbox;

                    // What should I do else, looks pretty screwed up??
                }
            }

            goto BAILOUT;
        }

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::setTimeout(&alrm, locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
#ifdef HAVE_WRESIZE

        while ( (ch = cancelbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
        if ( (ch == '\n' || ch == KEY_ESC) &&
             sureToCancel() ) {
            goto BAILOUT;
        }

#ifdef ENABLE_PWGEN
#  ifdef HAVE_WRESIZE

        while ( (ch = pwgenbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#  else // HAVE_WRESIZE
        ch = pwgenbutton->focus();
#  endif // HAVE_WRESIZE

        if (ch == '\n') {
            PWGenDialog* tmp = NULL;

            try {
                tmp = new PWGenDialog();
                tmp->run();

                if (!tmp->isCanceled() ) {
                    s_password = tmp->getPassword();
                    passwordchanged = true;
                    password->setText(s_password);
                    password->setTextChanged(true);
                }

                delete tmp;
            } catch (std::exception& ex2) {
                if (tmp != NULL)
                    delete tmp;
            }

            YAPET::UI::BaseWindow::refreshAll();
        }

        if (ch == KEY_ESC && sureToCancel() ) {
            goto BAILOUT;
        }

#endif // ENABLE_PWGEN
    } // while (true)

BAILOUT:
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
    YAPET::UI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
}

void
PasswordRecord::resize() throw(YAPET::UI::UIException) {
    int retval = delwin(window);

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error deleting password entry window") );


    delete name;
    delete host;
    delete username;
    delete password;
    delete comment;
    delete okbutton;
    delete cancelbutton;
#ifdef ENABLE_PWGEN
    delete pwgenbutton;
#endif
    window = NULL;
    name = NULL;
    username = NULL;
    password = NULL;
    comment = NULL;
    okbutton = NULL;
    cancelbutton = NULL;
#ifdef ENABLE_PWGEN
    pwgenbutton = NULL;
#endif
    createWindow();

    // Reset the resize_due attribute
    resize_due = false;
}

void
PasswordRecord::refresh() throw(YAPET::UI::UIException) {
    YAPET::UI::Colors::setcolor(window, YAPET::UI::MESSAGEBOX);
    int retval = wclear(window);

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error clearing window") );

    retval = box(window, 0, 0);

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding box") );

    if (readonly)
        retval =
            mymvwaddstr(window, 0, 2, _(
                            "P A S S W O R D  R E C O R D (Read-Only)") );
    else
        retval =
            mymvwaddstr(window, 0, 2, _("P A S S W O R D  R E C O R D") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error setting label") );

    retval = mymvwaddstr(window, 1, 1, _("Name") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding label") );

    retval = mymvwaddstr(window, 3, 1, _("Host") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding label") );

    retval = mymvwaddstr(window, 5, 1, _("Username") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding label") );

    retval = mymvwaddstr(window, 7, 1, _("Password") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding label") );

    retval = mymvwaddstr(window, 9, 1, _("Comment") );

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error adding label") );

    retval = wrefresh(window);

    if (retval == ERR)
        throw YAPET::UI::UIException(_("Error refreshing window") );

    name->setText(s_name);
    name->setTextChanged(namechanged);
    name->refresh();
    host->setText(s_host);
    host->setTextChanged(hostchanged);
    host->refresh();
    username->setText(s_username);
    username->setTextChanged(usernamechanged);
    username->refresh();
    password->setText(s_password);
    password->setHidden(readonly);
    password->setTextChanged(passwordchanged);
    password->refresh();
    comment->setText(s_comment);
    comment->setTextChanged(commentchanged);
    comment->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
#ifdef ENABLE_PWGEN
    pwgenbutton->refresh();
#endif
}

bool
PasswordRecord::entryChanged() const {
    return name->isTextChanged() ||
           host->isTextChanged() ||
           username->isTextChanged() ||
           password->isTextChanged() ||
           comment->isTextChanged();
}

void
PasswordRecord::setReadonly(bool ro) {
    readonly = ro;
    name->setReadonly(readonly);
    host->setReadonly(readonly);
    username->setReadonly(readonly);
    password->setReadonly(readonly);
    password->setHidden(readonly);
    comment->setReadonly(readonly);
#ifdef ENABLE_PWGEN
    pwgenbutton->setReadonly(readonly);
#endif
}
#endif // #if 0
