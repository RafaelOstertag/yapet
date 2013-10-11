// -*- c++ -*-
//
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

#ifndef _PASSWORDDIALOG_H
#define _PASSWORDDIALOG_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "secstring.h"

#include "yacurs.h"

/**
 * @brief Determines the type of the password dialog.
 *
 * Determines the type of the password dialog: Either a password
 * dialog for a new password or a dialog for an existing password.
 */
enum PWTYPE {
    /**
     * Makes the \c PasswordDialog show two input widget for the
     * password. One for the password and the other to confirm the
     * password.
     */
    NEW_PW,

    /**
     * Makes the \c PasswordDialog show only one input widget for the
     * password.
     */
    EXISTING_PW
};

/**
 * @brief Shows a dialog for entering the password.
 *
 * Depending on the \c PWTYPE, it shows either one or two password
 * input widgets. If \c PWTYPE is \c NEW_PW, it shows two input
 * widgets, one for the password and the other to confirm the
 * password. If the passwords matches and the user doesn't cancel the
 * dialog, a \c Key is generated and put on the heap. The pointer to
 * the key can be obtained by calling \c getKey(). The memory occupied
 * by this key is NOT freed by this class.
 *
 * If \c PWTYPE is \c EXISTING_PW, only one widget for entering the
 * password is displayed. The same rules as for \c NEW_PW apply in
 * regard to the key.
 */
class PasswordDialog : public YACURS::Dialog  {
    private:
        YACURS::Input<std::string>* pwinput1;
        YACURS::Input<std::string>* pwinput2;
	YACURS::Label* line1;
	YACURS::Label* linefn;
	YACURS::Label* line2;
        PWTYPE __pwtype;

        const PasswordDialog& operator=(const PasswordDialog&) {
            return *this;
        }

    public:
        PasswordDialog(PWTYPE pt,
		       std::string& fn);
        ~PasswordDialog();

	PWTYPE pwtype() const { return __pwtype; }
	
	bool password_match() const {
	    if (__pwtype==EXISTING_PW)
		return pwinput1->input() == pwinput2->input();
	    else
		return true;
	}

	std::string password(bool* match) const {
	    if (match!=0)
		*match = password_match();

	    return pwinput1->input();
	}
};

#endif // _PASSWORDDIALOG_H
