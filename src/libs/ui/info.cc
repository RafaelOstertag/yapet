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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CRYPTO_H
#include <openssl/crypto.h>
#endif

#include <sys/resource.h>

#include <sstream>

#include "filehelper.hh"
#include "globals.h"
#include "info.h"
#include "intl.h"

//
// Private
//

//
// Protected
//

//
// Public
//
InfoDialog::InfoDialog(const MainWindow& mainWindow,
                       YACURS::ListBox<>::lsz_t entries)
    : YACURS::Dialog{_("Information"), YACURS::OK_ONLY, YACURS::AUTOMATIC},
      mainpack{},
      leftpack{},
      rightpack{},
      fn{_("File: ")},
      fn_status{std::string()},
      fp{_("File permission check: ")},
      slt{_("Screen lock time-out: ")},
      slt_status{},
      sult{_("Unlock Dialog time-out: ")},
      sult_status{},
      ent{_("Password records: ")},
      ent_status{},
      mod{_("Records modified: ")},
      mod_status{},
      ver{_("PET file version: ")},
      ver_status{},
      pwset{_("Password set: ")},
      pwset_status{},
      arch{_("Architecture: ")},
      arch_status{},
      ssl{"OpenSSL:"},
#if defined(HAVE_SSLEAY_VERSION)
      ssl_status{SSLeay_version(SSLEAY_VERSION)},
#elif defined(HAVE_OPENSSL_VERSION)
      ssl_status{OpenSSL_version(OPENSSL_VERSION)},
#else
      ssl_status{_("n/a")},
#endif

      cfs{_("Core File:")},
#if defined(RLIMIT_CORE) && !defined(DEBUG)
      cfs_status{_("suppressed")},
#else
      cfs_status{_("not suppressed")},
#endif
      argon2Memory{_("Argon2 Memory")},
      argon2MemoryStatus{_("n/a")},
      argon2Paralleism{_("Argon2 Parallelism")},
      argon2ParalleismStatus{_("n/a")},
      argon2Iterations{_("Argon2 Iterations")},
      argon2IterationsStatus{_("n/a")} {
    // used to make the file name label get the entire space available
    // to the pack.
    rightpack.always_dynamic(true);
    rightpack.hinting(false);

    mainpack.add_back(&leftpack);
    mainpack.add_back(&rightpack);
    widget(&mainpack);

    leftpack.add_back(&fn);
    if (!mainWindow.currentFilename().empty())
        fn_status.label(mainWindow.currentFilename());
    else
        fn_status.label(_("none loaded"));
    rightpack.add_back(&fn_status);

    leftpack.add_back(&fp);
    if (YAPET::Globals::config.filesecurity)
        fp_status.label(_("enabled"));
    else
        fp_status.label(_("disabled"));
    rightpack.add_back(&fp_status);

    leftpack.add_back(&slt);
    std::ostringstream val;
    val << YAPET::Globals::config.timeout;
    slt_status.label(val.str());
    rightpack.add_back(&slt_status);

    leftpack.add_back(&sult);
    val.str("");
    val.clear();
    val << YAPET::Globals::config.pw_input_timeout;
    sult_status.label(val.str());
    rightpack.add_back(&sult_status);

    leftpack.add_back(&ent);
    val.str("");
    val.clear();
    val << entries;
    ent_status.label(val.str());
    rightpack.add_back(&ent_status);

    leftpack.add_back(&mod);
    if (YAPET::Globals::records_changed)
        mod_status.label(_("yes"));
    else
        mod_status.label(_("no"));
    rightpack.add_back(&mod_status);

    leftpack.add_back(&ver);
    auto fileVersion{mainWindow.fileVersion()};
    if (!fileVersion.empty()) {
        val.str("");
        val.clear();
        val << fileVersion;
        ver_status.label(val.str());
    } else {
        ver_status.label(_("n/a"));
    }
    rightpack.add_back(&ver_status);

    leftpack.add_back(&pwset);
    if (mainWindow.passwordLastChanged() != -1) {
        time_t t = static_cast<time_t>(mainWindow.passwordLastChanged());
        pwset_status.label(asctime(localtime(&t)));
    } else {
        pwset_status.label(_("n/a"));
    }
    rightpack.add_back(&pwset_status);

    leftpack.add_back(&arch);
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
    arch_status.label(archstr);
    rightpack.add_back(&arch_status);

    leftpack.add_back(&ssl);
    rightpack.add_back(&ssl_status);

    leftpack.add_back(&cfs);
    rightpack.add_back(&cfs_status);

    if (!mainWindow.currentFilename().empty()) {
        try {
            auto metaData{
                yapet::readMetaData(mainWindow.currentFilename(), false)};

            argon2IterationsStatus.label(std::to_string(
                metaData.getValue(YAPET::Consts::ARGON2_TIME_COST_KEY)));
            argon2MemoryStatus.label(std::to_string(
                metaData.getValue(YAPET::Consts::ARGON2_MEMORY_COST_KEY)));
            argon2ParalleismStatus.label(std::to_string(
                metaData.getValue(YAPET::Consts::ARGON2_PARALLELISM_KEY)));
        } catch (...) {
            // ok, maybe not a YAPET 2.0 file
        }
    }

    leftpack.add_back(&argon2Iterations);
    rightpack.add_back(&argon2IterationsStatus);

    leftpack.add_back(&argon2Memory);
    rightpack.add_back(&argon2MemoryStatus);

    leftpack.add_back(&argon2Paralleism);
    rightpack.add_back(&argon2ParalleismStatus);

    // colors
    fn.color(YACURS::DIALOG);
    fn_status.color(YACURS::DIALOG);

    fp.color(YACURS::DIALOG);
    fp_status.color(YACURS::DIALOG);

    slt.color(YACURS::DIALOG);
    slt_status.color(YACURS::DIALOG);

    sult.color(YACURS::DIALOG);
    sult_status.color(YACURS::DIALOG);

    ent.color(YACURS::DIALOG);
    ent_status.color(YACURS::DIALOG);

    mod.color(YACURS::DIALOG);
    mod_status.color(YACURS::DIALOG);

    ver.color(YACURS::DIALOG);
    ver_status.color(YACURS::DIALOG);

    pwset.color(YACURS::DIALOG);
    pwset_status.color(YACURS::DIALOG);

    arch.color(YACURS::DIALOG);
    arch_status.color(YACURS::DIALOG);

    ssl.color(YACURS::DIALOG);
    ssl_status.color(YACURS::DIALOG);

    cfs.color(YACURS::DIALOG);
    cfs_status.color(YACURS::DIALOG);

    argon2Iterations.color(YACURS::DIALOG);
    argon2IterationsStatus.color(YACURS::DIALOG);

    argon2Memory.color(YACURS::DIALOG);
    argon2MemoryStatus.color(YACURS::DIALOG);

    argon2Paralleism.color(YACURS::DIALOG);
    argon2ParalleismStatus.color(YACURS::DIALOG);
}

InfoDialog::~InfoDialog() {}
