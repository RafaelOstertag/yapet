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

#ifndef _PASSWORDDIALOG_H
#define _PASSWORDDIALOG_H

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

#include <key.h>

#include <resizeable.h>
#include <button.h>
#include <passwordwidget.h>

enum PWTYPE {
    NEW_PW,
    EXISTING_PW
};

class PasswordDialog : protected GPSUI::Resizeable {
    private:
	enum {
	    HEIGHT_NEW = 9,
	    HEIGHT_EX = 7
	};

	WINDOW* window;
	GPSUI::PasswordWidget* pwidget1;
	GPSUI::PasswordWidget* pwidget2;
	GPSUI::Button* okbutton;
	GPSUI::Button* cancelbutton;
	PWTYPE pwtype;
	GPSAFE::Key* key;

	std::string filename;

	inline PasswordDialog(const PasswordDialog&) {}
	inline const PasswordDialog& operator=(const PasswordDialog&) { return *this; }

	inline int getWidth() const {
	    return maxX() - 8;
	}

	inline int getHeight() const {
	    if (pwtype == NEW_PW) 
		return HEIGHT_NEW;
	    else
		return HEIGHT_EX;
	}

	inline int getStartX() const {
	    return maxX()/2 - getWidth()/2;
	}

	inline int getStartY() const {
	    return maxY()/2 - getHeight()/2;
	}
	    
	void createWindow() throw(GPSUI::UIException);

    public:
	PasswordDialog(PWTYPE pt, std::string fn) throw(GPSUI::UIException);
	~PasswordDialog();

	void run() throw(GPSUI::UIException);
	/**
	 * Caller is responsible for freeing the key
	 */
	GPSAFE::Key* getKey() const { return key; }
	void resize() throw(GPSUI::UIException);
	void refresh() throw(GPSUI::UIException);
};

#endif // _PASSWORDDIALOG_H
