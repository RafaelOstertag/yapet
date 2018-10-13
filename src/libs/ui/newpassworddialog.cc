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

#include "newpassworddialog.h"
#include "intl.h"
#include "utils.hh"

#include <cassert>

//
// Private
//
bool NewPasswordDialog::on_close() {
    if (!match() && dialog_state() != YACURS::DIALOG_CANCEL) {
        assert(nomatchdialog == nullptr);
        nomatchdialog = new YACURS::MessageBox2(
            _("Password Missmatch"), _("Passwords do not match."),
            _("Do you want to retry?"), YACURS::YESNO);
        nomatchdialog->show();
        return false;
    }

    if (pwinput1->input().empty() && dialog_state() != YACURS::DIALOG_CANCEL) {
        assert(nomatchdialog == nullptr);
        nomatchdialog = new YACURS::MessageBox2(
            _("Password Missmatch"), _("Password must not be empty."),
            _("Do you want to retry?"), YACURS::YESNO);
        nomatchdialog->show();
        return false;
    }

    return true;
}

void NewPasswordDialog::window_close_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_WINDOW_CLOSE);

    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(_e);

    if (nomatchdialog == evt.data()) {
        if (nomatchdialog->dialog_state() == YACURS::DIALOG_YES) {
            pwinput1->clear();
            pwinput2->clear();
        } else {
            dialog_state(YACURS::DIALOG_CANCEL);
            close();
        }
        yapet::deleteAndZero(&nomatchdialog);
    }
}

//
// Public
//

NewPasswordDialog::NewPasswordDialog(std::string& fn)
    : Dialog{_("New Password")},
      nomatchdialog{nullptr},
      pwinput1{new YACURS::Input<>{}},
      pwinput2{new YACURS::Input<>{}},
      line1{new YACURS::Label{_("Enter password for ")}},
      line2{new YACURS::Label{_("Confirm password")}},
      linefn{new YACURS::Label{fn}} {
    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<NewPasswordDialog>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &NewPasswordDialog::window_close_handler));

    pwinput2->obscure_input(true);
    widget(pwinput2);

    widget(line2);

    pwinput1->obscure_input(true);
    widget(pwinput1);

    widget(linefn);  // add label to dialog
    widget(line1);   // add label to dialog

    linefn->color(YACURS::DIALOG);
    line1->color(YACURS::DIALOG);
    line2->color(YACURS::DIALOG);
}

NewPasswordDialog::~NewPasswordDialog() {
    assert(pwinput1 != nullptr);
    assert(pwinput2 != nullptr);
    assert(line1 != nullptr);
    assert(line2 != nullptr);
    assert(linefn != nullptr);

    if (nomatchdialog != nullptr) delete nomatchdialog;
    delete pwinput1;
    delete pwinput2;
    delete line1;
    delete line2;
    delete linefn;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<NewPasswordDialog>(
            YACURS::EVT_WINDOW_CLOSE, this,
            &NewPasswordDialog::window_close_handler));
}
