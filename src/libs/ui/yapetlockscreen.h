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

#ifndef _YAPETLOCKSCREEN_H
#define _YAPETLOCKSCREEN_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>
#include "mainwindow.h"

class YapetLockScreen : public YACURS::LockScreen {
   private:
    const MainWindow& _mainWindow;

   public:
    YapetLockScreen(YACURS::Window& mainWindow, YACURS::UnlockDialog* _unlock,
                    unsigned int timeout, unsigned int ulck_timeout);

    YapetLockScreen(const YapetLockScreen&) = delete;
    YapetLockScreen(YapetLockScreen&&) = delete;
    YapetLockScreen& operator=(const YapetLockScreen&) = delete;
    YapetLockScreen& operator=(YapetLockScreen&&) = delete;
    virtual ~YapetLockScreen();

    void show();

    void close();
};

#endif  // _YAPETLOCKSCREEN_H
