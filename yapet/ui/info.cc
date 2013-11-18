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

#include <sstream>

#include "../intl.h"
#include "globals.h"
#include "cfg.h"
#include "info.h"

//
// Private
//

//
// Protected
//

//
// Public
//
InfoDialog::InfoDialog(YACURS::ListBox<>::lsz_t entries):
    YACURS::Dialog(_("Information"), YACURS::OK_ONLY, YACURS::AUTOMATIC ) {
    mainpack=new YACURS::HPack;
    leftpack=new YACURS::VPack;
    rightpack=new YACURS::VPack;

    widget(mainpack);
    mainpack->add_back(leftpack);
    mainpack->add_back(rightpack);

    fp=new YACURS::Label(_("File permission check: "));
    leftpack->add_back(fp);

    if (YAPET::CONFIG::config.filesecurity)
	fp_status=new YACURS::Label(_("enabled"));
    else
	fp_status=new YACURS::Label(_("disabled"));
    rightpack->add_back(fp_status);

    slt=new YACURS::Label(_("Screen lock time-out: "));
    leftpack->add_back(slt);
    std::ostringstream val;
    val << YAPET::CONFIG::config.timeout;
    slt_status=new YACURS::Label(val.str());
    rightpack->add_back(slt_status);

    ent=new YACURS::Label(_("Password records: "));
    leftpack->add_back(ent);
    val.str("");
    val.clear();
    val << entries;
    ent_status=new YACURS::Label(val.str());
    rightpack->add_back(ent_status);

    mod=new YACURS::Label(_("Records modified: "));
    leftpack->add_back(mod);
    if (YAPET::Globals::records_changed)
	mod_status=new YACURS::Label(_("yes"));
    else
	mod_status=new YACURS::Label(_("no"));
    rightpack->add_back(mod_status);

    ver=new YACURS::Label(_("PET file version: "));
    leftpack->add_back(ver);
    if (YAPET::Globals::file != 0 && YAPET::Globals::key != 0) {
	val.str("");
	val.clear();
	val << YAPET::Globals::file->getFileVersion(*YAPET::Globals::key);
	ver_status=new YACURS::Label(val.str());
    } else {
	ver_status=new YACURS::Label(_("n/a"));
    }
    rightpack->add_back(ver_status);

#if defined(HAVE_ASCTIME) && defined(HAVE_LOCALTIME)
    pwset=new YACURS::Label(_("Password set: "));
    leftpack->add_back(pwset);
    if (YAPET::Globals::file != 0 && YAPET::Globals::key != 0) {
	time_t t= static_cast<time_t>(YAPET::Globals::file->getMasterPWSet (*YAPET::Globals::key));
	pwset_status=new YACURS::Label(asctime(localtime(&t)));
    } else {
	pwset_status=new YACURS::Label(_("n/a"));
    }
    rightpack->add_back(pwset_status);
#endif

    fp->color(YACURS::DIALOG);
    fp_status->color(YACURS::DIALOG);
   
    slt->color(YACURS::DIALOG);
    slt_status->color(YACURS::DIALOG);

    ent->color(YACURS::DIALOG);
    ent_status->color(YACURS::DIALOG);
   
    mod->color(YACURS::DIALOG);
    mod_status->color(YACURS::DIALOG);

    ver->color(YACURS::DIALOG);
    ver_status->color(YACURS::DIALOG);

#if defined(HAVE_ASCTIME) && defined(HAVE_LOCALTIME)
    pwset->color(YACURS::DIALOG);
    pwset_status->color(YACURS::DIALOG);
#endif
}

InfoDialog::~InfoDialog() {
    delete mainpack;
    delete leftpack;
    delete rightpack;

    delete fp;
    delete fp_status;

    delete slt;
    delete slt_status;

    delete ent;
    delete ent_status;

    delete mod;
    delete mod_status;

    delete ver;
    delete ver_status;

#if defined(HAVE_ASCTIME) && defined(HAVE_LOCALTIME)
    delete pwset;
    delete pwset_status;
#endif
}

