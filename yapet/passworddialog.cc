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

#include "../intl.h"
#include "passworddialog.h"

#include <cassert>

void
PasswordDialog::button_press_handler(YACURS::Event& _e) {
  
    YACURS::EventEx<YACURS::Button*>& e =
	dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    
}

PasswordDialog::PasswordDialog(PWTYPE pt, std::string& fn) : Dialog(_("Enter Password")),
					    pwinput1(new YACURS::Input<>()),
							     pwinput2(pt==NEW_PW? new YACURS::Input<>() : 0),
					    line1(new YACURS::Label(_("Enter password for "))),
					    linefn(new YACURS::Label(fn)),
							     line2(pt==NEW_PW? new YACURS::Label(_("Confirm password")): 0),
					    __pwtype(pt) {
					    
    widget(line1); // add label to dialog
    widget(linefn); // add label to dialog
    
    pwinput1->obscure_input(true); 
    widget(pwinput1);
    
    if (__pwtype==NEW_PW) {
	widget(line2);
	
	pwinput2->obscure_input(true);
	widget(pwinput2);
    }
							    }

PasswordDialog::~PasswordDialog() {
    assert(pwinput1!=0);
    assert(line1!=0);
    assert(linefn!=0);

    delete pwinput1;
    delete line1;
    delete linefn;

    if (line2!=0) delete line2;
    if (pwinput2!=0) delete pwinput2;
}
