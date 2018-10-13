/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "baseerror.hh"
#include "globals.h"
#include "intl.h"
#include "passwordrecord.h"
#include "utils.hh"

class HotKeyCtrlR : public YACURS::HotKey {
   private:
    PasswordRecord* ptr;

   public:
    HotKeyCtrlR(PasswordRecord* p) : HotKey{18}, ptr{p} {
        assert(p != nullptr);
    }
    HotKeyCtrlR(const HotKeyCtrlR& hk) : HotKey{hk}, ptr{hk.ptr} {}
    void action() { ptr->readonly(!ptr->readonly()); }

    HotKey* clone() const { return new HotKeyCtrlR(*this); }
};

class HotKeyCtrlT : public YACURS::HotKey {
   private:
    PasswordRecord* ptr;

   public:
    HotKeyCtrlT(PasswordRecord* p) : HotKey{20}, ptr{p} {
        assert(p != nullptr);
    }
    HotKeyCtrlT(const HotKeyCtrlT& hk) : HotKey{hk}, ptr{hk.ptr} {}
    void action() { ptr->password_hidden(!ptr->password_hidden()); }

    HotKey* clone() const { return new HotKeyCtrlT(*this); }
};

void PasswordRecord::on_ok_button() {
    yapet::PasswordRecord passwordRecord;
    passwordRecord.name(name->input().c_str());
    passwordRecord.host(host->input().c_str());
    passwordRecord.username(username->input().c_str());
    passwordRecord.password(password->input().c_str());
    passwordRecord.comment(comment->input().c_str());

    try {
        auto serializedPasswordRecord{passwordRecord.serialize()};
        auto crypto{_cryptoFactory->crypto()};
        auto encryptedPasswordRecord{crypto->encrypt(serializedPasswordRecord)};
        _passwordListItem = std::shared_ptr<yapet::PasswordListItem>{
            new yapet::PasswordListItem{name->input().c_str(),
                                        encryptedPasswordRecord}};
    } catch (yapet::YAPETBaseError& ex) {
        errordialog = new YACURS::MessageBox(_("Error"), ex.what());
        errordialog->show();
    }
}

//
// Private
//

void PasswordRecord::button_press_handler(YACURS::Event& e) {
    Dialog::button_press_handler(e);

    YACURS::EventEx<YACURS::Button*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(e);

    if (evt.data() == pwgenbutton) {
        assert(pwgendialog == nullptr);
        pwgendialog = new PwGenDialog();
        pwgendialog->show();
    }
}

void PasswordRecord::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (errordialog != nullptr && evt.data() == errordialog) {
        yapet::deleteAndZero(&errordialog);
        return;
    }

    // Handle the confirmation of whether dialog should be closed
    // (cancelled) with pending changes.
    if (confirmdialog != nullptr && evt.data() == confirmdialog) {
        if (confirmdialog->dialog_state() == YACURS::DIALOG_YES) {
            _force_close = true;
            close();
        }
        yapet::deleteAndZero(&confirmdialog);
        return;
    }

    if (evt.data() == pwgendialog) {
        assert(!_readonly);
        if (pwgendialog->dialog_state() == YACURS::DIALOG_OK) {
            password->input(pwgendialog->password());
            _modified_by_pwgen = true;
        }

        yapet::deleteAndZero(&pwgendialog);
        return;
    }
}

bool PasswordRecord::on_close() {
    if (changed() && dialog_state() != YACURS::DIALOG_OK && !_force_close) {
        assert(confirmdialog == nullptr);
        confirmdialog = new YACURS::MessageBox(
            _("Pending Changes"), _("Do you want to discard changes?"),
            YACURS::YESNO);
        confirmdialog->show();
        return false;
    }
    return true;
}

//
// Public
//

PasswordRecord::PasswordRecord(
    std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory,
    const yapet::PasswordListItem& passwordListItem)
    : PasswordRecord{cryptoFactory} {
    _newrecord = false;
    try {
        auto crypto{_cryptoFactory->crypto()};
        auto decryptedSerializedRecord{
            crypto->decrypt(passwordListItem.encryptedRecord())};
        yapet::PasswordRecord passwordRecord{decryptedSerializedRecord};
        // This also sets the _password_hidden attribute
        readonly(true);
        name->input(reinterpret_cast<const char*>(passwordRecord.name()));
        host->input(reinterpret_cast<const char*>(passwordRecord.host()));
        username->input(
            reinterpret_cast<const char*>(passwordRecord.username()));
        password->input(
            reinterpret_cast<const char*>(passwordRecord.password()));
        comment->input(reinterpret_cast<const char*>(passwordRecord.comment()));
    } catch (yapet::YAPETBaseError& ex) {
        try {
            errordialog = new YACURS::MessageBox(_("Error"), ex.what());
            errordialog->show();
        } catch (YACURS::EXCEPTIONS::BaseCurEx&) {
            if (errordialog != nullptr) delete errordialog;

            // What should I do else, looks pretty screwed up??
        }
    }
}

PasswordRecord::PasswordRecord(
    std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory)
    : YACURS::Dialog{_("Password Entry")},
      vpack{new YACURS::VPack},
      lname{new YACURS::Label{_("Name")}},
      lhost{new YACURS::Label{_("Host")}},
      lusername{new YACURS::Label{_("User Name")}},
      lpassword{new YACURS::Label{_("Password")}},
      lcomment{new YACURS::Label{_("Comment")}},
      name{new YACURS::Input<std::string>},
      host{new YACURS::Input<std::string>},
      username{new YACURS::Input<std::string>},
      password{new YACURS::Input<std::string>},
      comment{new YACURS::Input<std::string>},
      pwgenbutton{new YACURS::Button{_("Password Generator")}},
      pwgenbutton_spacer{new YACURS::Spacer},
      errordialog{nullptr},
      confirmdialog{nullptr},
      pwgendialog{nullptr},
      _cryptoFactory{cryptoFactory},
      _passwordListItem{nullptr},
      _newrecord{true},
      _readonly{false},
      _password_hidden{false},
      _force_close{false},
      _modified_by_pwgen{false} {
    name->max_input(yapet::PasswordRecord::NAME_SIZE);
    host->max_input(yapet::PasswordRecord::HOST_SIZE);
    username->max_input(yapet::PasswordRecord::USERNAME_SIZE);
    password->max_input(yapet::PasswordRecord::PASSWORD_SIZE);
    comment->max_input(yapet::PasswordRecord::COMMENT_SIZE);

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

    add_button(pwgenbutton_spacer);
    add_button(pwgenbutton);

    widget(vpack);

    add_hotkey(HotKeyCtrlR(this));
    add_hotkey(HotKeyCtrlT(this));

    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<PasswordRecord>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &PasswordRecord::window_close_handler));
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
    delete pwgenbutton;
    delete pwgenbutton_spacer;
    delete vpack;

    if (errordialog) delete errordialog;
    if (confirmdialog) delete confirmdialog;
    if (pwgendialog) delete pwgendialog;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<PasswordRecord>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &PasswordRecord::window_close_handler));
}

bool PasswordRecord::changed() const {
    return name->changed() || host->changed() || username->changed() ||
           password->changed() || comment->changed() || _modified_by_pwgen;
}

void PasswordRecord::readonly(bool f) {
    name->readonly(f);
    host->readonly(f);
    username->readonly(f);
    password->readonly(f);
    password->hide_input(f);
    comment->readonly(f);
    _password_hidden = _readonly = f;

    if (_readonly)
        title(_("Password Entry (Read-Only)"));
    else
        title(_("Password Entry"));

    pwgenbutton->enabled(!_readonly);
}

void PasswordRecord::password_hidden(bool f) {
    // We only allow to hide the password when dialog is in read-only
    // mode.

    if (!_readonly) return;

    password->hide_input(_password_hidden = f);
}
