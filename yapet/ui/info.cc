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

#ifdef HAVE_CRYPTO_H
# include <openssl/crypto.h>
#endif

#include <sstream>

#include "../intl.h"
#include "globals.h"
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

    if (YAPET::Globals::config.filesecurity)
	fp_status=new YACURS::Label(_("enabled"));
    else
	fp_status=new YACURS::Label(_("disabled"));
    rightpack->add_back(fp_status);

    slt=new YACURS::Label(_("Screen lock time-out: "));
    leftpack->add_back(slt);
    std::ostringstream val;
    val << YAPET::Globals::config.timeout;
    slt_status=new YACURS::Label(val.str());
    rightpack->add_back(slt_status);

    sult=new YACURS::Label(_("Unlock Dialog time-out: "));
    leftpack->add_back(sult);
    val.str("");
    val.clear();
    val << YAPET::Globals::config.pw_input_timeout;
    sult_status=new YACURS::Label(val.str());
    rightpack->add_back(sult_status);

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

    arch=new YACURS::Label(_("Architecture: "));
    leftpack->add_back(arch);
    std::string archstr;
#if defined(SIZEOF_INT_P)
    val.str("");
    val.clear();
    val << 8 * SIZEOF_INT_P;
    archstr = val.str();
    archstr += "bit ";
#endif
#if defined(WORDS_BIGENDIAN)
    archstr += _("big endian");
#else
    archstr += _("little endian");
#endif
    arch_status=new YACURS::Label(archstr);
    rightpack->add_back(arch_status);

    ssl=new YACURS::Label("OpenSSL:");
    leftpack->add_back(ssl);
#if defined(HAVE_SSLEAY_VERSION)
    ssl_status=new YACURS::Label(SSLeay_version(SSLEAY_VERSION));
#else
    ssl_status=new YACURS::Label(_("n/a"));
#endif
    rightpack->add_back(ssl_status);

    ttl=new YACURS::Label(_("Terminal Title:"));
    leftpack->add_back(ttl);
#if defined(HAVE_TERMINALTITLE)
    ttl_status=new YACURS::Label(_("built-in"));
#else
    ttl_status=new YACURS::Label(_("not built-in"));
#endif
    rightpack->add_back(ttl_status);

    pwg=new YACURS::Label(_("Password Generator:"));
    leftpack->add_back(pwg);
#if defined(ENABLE_PWGEN)
    pwg_status=new YACURS::Label(_("built-in"));
#else
    pwg_status=new YACURS::Label(_("not built-in"));
#endif
    rightpack->add_back(pwg_status);

    cfs=new YACURS::Label(_("Core File:"));
    leftpack->add_back(cfs);
#if defined(HAVE_SETRLIMIT) && defined(RLIMIT_CORE)
    cfs_status=new YACURS::Label(_("suppressed"));
#else
    cfs_status=new YACURS::Label(_("not suppressed"));
#endif
    rightpack->add_back(cfs_status);

    // colors
    fp->color(YACURS::DIALOG);
    fp_status->color(YACURS::DIALOG);

    slt->color(YACURS::DIALOG);
    slt_status->color(YACURS::DIALOG);

    sult->color(YACURS::DIALOG);
    sult_status->color(YACURS::DIALOG);

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

    arch->color(YACURS::DIALOG);
    arch_status->color(YACURS::DIALOG);

    ssl->color(YACURS::DIALOG);
    ssl_status->color(YACURS::DIALOG);

    ttl->color(YACURS::DIALOG);
    ttl_status->color(YACURS::DIALOG);

    pwg->color(YACURS::DIALOG);
    pwg_status->color(YACURS::DIALOG);

    cfs->color(YACURS::DIALOG);
    cfs_status->color(YACURS::DIALOG);
}

InfoDialog::~InfoDialog() {
    delete mainpack;
    delete leftpack;
    delete rightpack;

    delete fp;
    delete fp_status;

    delete slt;
    delete slt_status;

    delete sult;
    delete sult_status;

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

    delete arch;
    delete arch_status;

    delete ssl;
    delete ssl_status;

    delete ttl;
    delete ttl_status;

    delete pwg;
    delete pwg_status;

    delete cfs;
    delete cfs_status;
}
