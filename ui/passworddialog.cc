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

#include "intl.h"
#include "passworddialog.h"

#include <cassert>

PasswordDialog::PasswordDialog(std::string& fn) : Dialog(_("Enter Password")),
						  pwinput1(new YACURS::Input<>()),
						  line1(new YACURS::Label(_("Enter password for "))),
						  linefn(new YACURS::Label(fn)) {
    pwinput1->obscure_input(true); 
    widget(pwinput1);
    
    widget(linefn); // add label to dialog
    widget(line1); // add label to dialog
    
    linefn->color(YACURS::DIALOG);
    line1->color(YACURS::DIALOG);
}

PasswordDialog::~PasswordDialog() {
    assert(pwinput1!=0);
    assert(line1!=0);
    assert(linefn!=0);

    delete pwinput1;
    delete line1;
    delete linefn;
}
