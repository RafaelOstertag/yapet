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

#include "intl.h"
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
    YACURS::Dialog(_("Help"), YACURS::OK_ONLY, YACURS::AUTOMATIC ) {
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

    createkey=new YACURS::Label(" E ");
    leftpack->add_back(createkey);
    createdescr=new YACURS::Label(_("Create File"));
    rightpack->add_back(createdescr);

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

    infokey=new YACURS::Label(" I ");
    leftpack->add_back(infokey);
    infodescr=new YACURS::Label(_("Information"));
    rightpack->add_back(infodescr);

    redrawkey=new YACURS::Label(" ^L ");
    leftpack->add_back(redrawkey);
    redrawdescr=new YACURS::Label(_("Redraw Screen"));
    rightpack->add_back(redrawdescr);

    pwgenkey=new YACURS::Label(" G ");
    leftpack->add_back(pwgenkey);
    pwgendescr=new YACURS::Label(_("Password Generator"));
    rightpack->add_back(pwgendescr);

    quitkey=new YACURS::Label(" Q ");
    leftpack->add_back(quitkey);
    quitdescr=new YACURS::Label(_("Quit"));
    rightpack->add_back(quitdescr);

    savekey->color(YACURS::DIALOG);
    savedescr->color(YACURS::DIALOG);
   
    loadkey->color(YACURS::DIALOG);
    loaddescr->color(YACURS::DIALOG);

    createkey->color(YACURS::DIALOG);
    createdescr->color(YACURS::DIALOG);
   
    lockkey->color(YACURS::DIALOG);
    lockdescr->color(YACURS::DIALOG);

    addkey->color(YACURS::DIALOG);
    adddescr->color(YACURS::DIALOG);

    delkey->color(YACURS::DIALOG);
    deldescr->color(YACURS::DIALOG);

    sortkey->color(YACURS::DIALOG);
    sortdescr->color(YACURS::DIALOG);

    searchkey->color(YACURS::DIALOG);
    searchdescr->color(YACURS::DIALOG);

    searchnkey->color(YACURS::DIALOG);
    searchndescr->color(YACURS::DIALOG);

    chpwkey->color(YACURS::DIALOG);
    chpwdescr->color(YACURS::DIALOG);

    infokey->color(YACURS::DIALOG);
    infodescr->color(YACURS::DIALOG);

    redrawkey->color(YACURS::DIALOG);
    redrawdescr->color(YACURS::DIALOG);

    quitkey->color(YACURS::DIALOG);
    quitdescr->color(YACURS::DIALOG);

    pwgenkey->color(YACURS::DIALOG);
    pwgendescr->color(YACURS::DIALOG);
}

HelpDialog::~HelpDialog() {
    delete mainpack;
    delete leftpack;
    delete rightpack;

    delete savekey;
    delete savedescr;

    delete loadkey;
    delete loaddescr;

    delete createkey;
    delete createdescr;

    delete lockkey;
    delete lockdescr;

    delete addkey;
    delete adddescr;

    delete delkey;
    delete deldescr;

    delete sortkey;
    delete sortdescr;

    delete searchkey;
    delete searchdescr;

    delete searchnkey;
    delete searchndescr;

    delete chpwkey;
    delete chpwdescr;

    delete infokey;
    delete infodescr;

    delete redrawkey;
    delete redrawdescr;

    delete pwgenkey;
    delete pwgendescr;

    delete quitkey;
    delete quitdescr;
}

