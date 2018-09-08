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

#ifndef _INFO_H
#define _INFO_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <mainwindow.h>
#include <yacurs.h>

/**
 */
class InfoDialog : public YACURS::Dialog {
   private:
    YACURS::HPack mainpack;
    YACURS::VPack leftpack;
    YACURS::VPack rightpack;

    YACURS::Label fn;
    YACURS::DynLabel fn_status;

    YACURS::Label fp;
    YACURS::Label fp_status;

    YACURS::Label slt;
    YACURS::Label slt_status;

    YACURS::Label sult;
    YACURS::Label sult_status;

    YACURS::Label ent;
    YACURS::Label ent_status;

    YACURS::Label mod;
    YACURS::Label mod_status;

    YACURS::Label ver;
    YACURS::Label ver_status;

    YACURS::Label pwset;
    YACURS::Label pwset_status;

    YACURS::Label arch;
    YACURS::Label arch_status;

    YACURS::Label ssl;
    YACURS::Label ssl_status;

    YACURS::Label cfs;
    YACURS::Label cfs_status;

   public:
    InfoDialog(const MainWindow& mainWindow, YACURS::ListBox<>::lsz_t entries);
    virtual ~InfoDialog();
};

#endif  // _INFO_H
