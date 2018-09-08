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

#ifndef _PASSWORDDIALOG_H
#define _PASSWORDDIALOG_H 1

#include "yacurs.h"

/**
 * @brief Shows a dialog for entering the password for an existsing YAPET file.
 */
class PasswordDialog : public YACURS::Dialog {
   private:
    YACURS::Input<std::string>* pwinput1;
    YACURS::Label* line1;
    YACURS::Label* linefn;

   public:
    PasswordDialog(const std::string& filename);
    PasswordDialog(const PasswordDialog&) = delete;
    PasswordDialog(PasswordDialog&&) = delete;
    PasswordDialog& operator=(const PasswordDialog&) = delete;
    PasswordDialog& operator=(PasswordDialog&&) = delete;

    ~PasswordDialog();

    std::string password() const {
        assert(pwinput1 != 0);
        return pwinput1->input();
    }
};

#endif  // _PASSWORDDIALOG_H
