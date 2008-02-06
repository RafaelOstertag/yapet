// -*- c++ -*-
//
// $Id$
//
// @@REPLACE@@
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#ifdef HAVE_STRING
# include <string>
#endif

#include "resizeable.h"
#include "uiexception.h"
#include "scrdim.h"

class StatusBar : protected GPSUI::Resizeable,
		  protected GPSUI::ScrDim {
    private:
	WINDOW* statusbar;
	std::string message;
	
	inline StatusBar(const StatusBar&) {}
	inline const StatusBar& operator=(const StatusBar&) { return *this; }

    protected:
	void createWindow() throw(GPSUI::UIException);

    public:
	StatusBar() throw(GPSUI::UIException);
	virtual ~StatusBar();

	void putMsg(std::string msg) throw(GPSUI::UIException);
	std::string getMsg() const { return message; }
	void refresh();

	void resize();
};

#endif // _STATUSBAR_H
