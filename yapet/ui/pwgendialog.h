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

#ifndef _PWGENDIALOG_H
#define _PWGENDIALOG_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pwgen.h"

#include <yacurs.h>

/**
 */
class PwGenDialog : public YACURS::Dialog {
    private:
        YAPET::PWGEN::PWGen pwgen;
	YACURS::VPack mainpack;
	YACURS::HPack boxespack;


	YACURS::Label genpwlabel;
	YACURS::Input<> genpw;

	YACURS::Label pwlenlabel;
	YACURS::Input<> pwlen;

	YACURS::Button regenbutton;

	YACURS::CheckBox* charpools;
	YACURS::RadioBox* sources;

	std::string get_name_of_rng() const;

    protected:
	virtual void checkbox_selection_handler(YACURS::Event& _e);

	void button_press_handler(YACURS::Event& _e);

    public:
        PwGenDialog();
        virtual ~PwGenDialog();

	const std::string& password() const {
	    return genpw.input();
	}
};

#endif // _PWGENDIALOG_H
