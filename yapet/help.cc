// $Id: mainwindow.cc 5225 2013-06-24 17:06:49Z rafisol $
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
#include "help.h"

//
// Private
//

//
// Protected
//

//
// Public
//
HelpDialog::HelpDialog():
    YACURS::Dialog(_("Help"), OK_ONLY, AUTOMATIC ) {
mainpack=new YACURS::HPack;
leftpack=new YACURS::VPack;
rightpack=new YACURS::VPack;

widget(mainpack);
mainpack->add_back(leftpack);
mainpack->add_back(rightpack);

savekey=new YACURS::Label(" S ");
leftpack->add_back(savekey);
savedescr=new YACURS::Label(_("Save File"));
rightpack->add_back(savedescr);

loadkey=new YACURS::Label(" R ");
leftpack->add_back(loadkey);
loaddescr=new YACURS::Label(_("Load File"));
rightpack->add_back(loaddescr);

lockkey=new YACURS::Label(" L ");
leftpack->add_back(lockkey);
lockdescr=new YACURS::Label(_("Lock Screen"));
rightpack->add_back(lockdescr);

addkey=new YACURS::Label(" A ");
leftpack->add_back(addkey);
adddescr=new YACURS::Label(_("Add Entry"));
rightpack->add_back(adddescr);

delkey=new YACURS::Label(" D ");
leftpack->add_back(delkey);
deldescr=new YACURS::Label(_("Delete Entry"));
rightpack->add_back(deldescr);

sortkey=new YACURS::Label(" O ");
leftpack->add_back(sortkey);
sortdescr=new YACURS::Label(_("Sort Order"));
rightpack->add_back(sortdescr);

searchkey=new YACURS::Label(" / ");
leftpack->add_back(searchkey);
searchdescr=new YACURS::Label(_("Search"));
rightpack->add_back(searchdescr);

searchnkey=new YACURS::Label(" N ");
leftpack->add_back(searchnkey);
searchndescr=new YACURS::Label(_("Search Next"));
rightpack->add_back(searchndescr);

chpwkey=new YACURS::Label(" C ");
leftpack->add_back(chpwkey);
chpwdescr=new YACURS::Label(_("Change Password"));
rightpack->add_back(chpwdescr);

redrawkey=new YACURS::Label(" ^L ");
leftpack->add_back(redrawkey);
redrawdescr=new YACURS::Label(_("Redraw Screen"));
rightpack->add_back(redrawdescr);



}

HelpDialog::~HelpDialog() {

}

