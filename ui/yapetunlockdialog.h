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

#ifndef _YAPETUNLOCKDIALOG_H
#define _YAPETUNLOCKDIALOG_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>
#include "mainwindow.h"

class YapetUnlockDialog : public YACURS::UnlockDialog {
    private:
	MainWindow& mainwin;

	YACURS::VPack* __vpack;
	YACURS::DynLabel* __text1;
	YACURS::DynLabel* __text2;
	YACURS::DynLabel* __text3;
	YACURS::Input<>* __secret_input;
	YACURS::Button* __quit;
	
	YapetUnlockDialog& operator=(const YapetUnlockDialog&);

    protected:
	void window_show_handler(YACURS::Event& _e);

	// From dialog
	void button_press_handler(YACURS::Event& _e);

    public:
	YapetUnlockDialog(Window& mw);
	
	~YapetUnlockDialog();

	bool unlock();
	
	void clear();
};

#endif // _YAPETUNLOCKDIALOG_H
