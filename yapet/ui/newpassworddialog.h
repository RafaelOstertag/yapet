// -*- c++ -*-
//
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

#ifndef _NEWPASSWORDDIALOG_H
#define _NEWPASSWORDDIALOG_H 1

#include <stdexcept>
#include "../intl.h"
#include "yacurs.h"

/**
 * @brief Shows a dialog for entering a new password.
 *
 * Shows dialog for entering a new password.
 */
class NewPasswordDialog : public YACURS::Dialog  {
    private:
	YACURS::MessageBox2* nomatchdialog;
        YACURS::Input<std::string>* pwinput1;
        YACURS::Input<std::string>* pwinput2;
	YACURS::Label* line1;
	YACURS::Label* line2;
	YACURS::Label* linefn;

        const NewPasswordDialog& operator=(const NewPasswordDialog&) {
            return *this;
        }

	bool on_close();

	void window_close_handler(YACURS::Event& _e);

    public:
	/**
	 * Constructor
	 *
	 * @param fn file name for which password has to be set.
	 */
        NewPasswordDialog(std::string& fn);
        ~NewPasswordDialog();

	bool match() const {
	    return pwinput1->input() == pwinput2->input();
	}

	std::string password() const {
	    assert(pwinput1!=0);
	    if (dialog_state()!=YACURS::DIALOG_OK ||
		!match()) {
		// Now, it doesn't make sense to call password if
		// there is no match or the dialog is not in state
		// DIALOG_OK
		throw std::logic_error(_("Calling NewPasswordDialog::password() while dialog_state()!=DIALOG_OK or !match()"));
	    }
	    return pwinput1->input();
	}
};

#endif // _NEWPASSWORDDIALOG_H
