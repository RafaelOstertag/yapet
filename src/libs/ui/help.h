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

#ifndef _HELP_H
#define _HELP_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>

/**
 */
class HelpDialog : public YACURS::Dialog {
   private:
    YACURS::HPack* mainpack;
    YACURS::VPack* leftpack;
    YACURS::VPack* rightpack;

    YACURS::Label* savekey;
    YACURS::Label* savedescr;

    YACURS::Label* loadkey;
    YACURS::Label* loaddescr;

    YACURS::Label* createkey;
    YACURS::Label* createdescr;

    YACURS::Label* lockkey;
    YACURS::Label* lockdescr;

    YACURS::Label* addkey;
    YACURS::Label* adddescr;

    YACURS::Label* delkey;
    YACURS::Label* deldescr;

    YACURS::Label* sortkey;
    YACURS::Label* sortdescr;

    YACURS::Label* searchkey;
    YACURS::Label* searchdescr;

    YACURS::Label* searchnkey;
    YACURS::Label* searchndescr;

    YACURS::Label* chpwkey;
    YACURS::Label* chpwdescr;

    YACURS::Label* infokey;
    YACURS::Label* infodescr;

    YACURS::Label* redrawkey;
    YACURS::Label* redrawdescr;

    YACURS::Label* quitkey;
    YACURS::Label* quitdescr;

    YACURS::Label* pwgenkey;
    YACURS::Label* pwgendescr;

   public:
    HelpDialog();
    virtual ~HelpDialog();
};

#endif  // _HELP_H
