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

#include "../intl.h"
#include "globals.h"
#include "crypt.h"
#include "passwordrecord.h"

void
PasswordRecord::button_press_handler(YACURS::Event& e) {
    Dialog::button_press_handler(e);

#warning "To be implemented."
}

void
PasswordRecord::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (errordialog != 0 && evt.data() == errordialog) {
        delete errordialog;
        errordialog = 0;
        return;
    }

    if (evt.data() == this) {
        assert(0);
    }
}

PasswordRecord::PasswordRecord(const YAPET::Key* key, const YAPET::PartDec* pe)  :
    YACURS::Dialog(_("Password Entry") ),
    vpack(new YACURS::VPack),
    lname(new YACURS::Label(_("Name") ) ),
    lhost(new YACURS::Label(_("Host") ) ),
    lusername(new YACURS::Label(_("User Name") ) ),
    lpassword(new YACURS::Label(_("Password") ) ),
    lcomment(new YACURS::Label(_("Comment") ) ),
    name(new YACURS::Input<std::string> ),
    host(new YACURS::Input<std::string> ),
    username(new YACURS::Input<std::string>),
    password(new YACURS::Input<std::string>),
    comment(new YACURS::Input<std::string>),
#ifdef ENABLE_PWGEN
    pwgenbutton(new YACURS::Button(_("Password Generator") ) ),
#endif
    errordialog(0),
    encentry(0),
    __key(key),
    __readonly(false) {
    assert(__key!=0);

    name->max_input(128);
    host->max_input(256);
    username->max_input(256);
    password->max_input(256);
    comment->max_input(512);

    lname->color(YACURS::DIALOG);
    lhost->color(YACURS::DIALOG);
    lusername->color(YACURS::DIALOG);
    lpassword->color(YACURS::DIALOG);
    lcomment->color(YACURS::DIALOG);

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

    widget(vpack);

    if (pe != 0) {
        YAPET::Record<YAPET::PasswordRecord>* dec_rec = 0;

        try {
            YAPET::Crypt crypt(*__key);
            dec_rec = crypt.decrypt<YAPET::PasswordRecord>(
                pe->getEncRecord() );
	    YAPET::PasswordRecord* ptr_rec = *dec_rec;
            name->input(reinterpret_cast<char*>(ptr_rec->name) );
            host->input(reinterpret_cast<char*>(ptr_rec->host) );
            username->input(reinterpret_cast<char*>(ptr_rec->username) );
            password->input(reinterpret_cast<char*>(ptr_rec->password) );
            comment->input(reinterpret_cast<char*>(ptr_rec->comment) );
            delete dec_rec;
        } catch (YAPET::YAPETException& ex) {
            if (dec_rec != 0)
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

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                          PasswordRecord>(
                                          YACURS::EVT_WINDOW_CLOSE, this,
                                          &PasswordRecord::
                                          window_close_handler) );
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

    YACURS::EventQueue::disconnect_event(YACURS::EventConnectorMethod1<
                                             PasswordRecord>(
                                             YACURS::EVT_WINDOW_CLOSE, this,
                                             &PasswordRecord::
                                             window_close_handler) );
}

bool
PasswordRecord::changed() const {
    if (name == 0 ||
        host == 0 ||
        username == 0 ||
        password == 0 ||
        comment == 0)
        return false;

    return name->changed() &&
           host->changed() &&
           username->changed() &&
           password->changed() &&
           comment->changed();
}
