// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
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

#ifndef _PASSWORDRECORD_H
#define _PASSWORDRECORD_H

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
#include <partdec.h>

#include <resizeable.h>
#include <button.h>
#include <passwordwidget.h>

class PasswordRecord : protected YAPETUI::Resizeable {
    private:
	enum {
	    HEIGHT = 14
	};

	WINDOW* window;
	YAPETUI::InputWidget* name;
	YAPETUI::InputWidget* host;
	YAPETUI::InputWidget* username;
	YAPETUI::InputWidget* password;
	YAPETUI::InputWidget* comment;
	YAPETUI::Button* okbutton;
	YAPETUI::Button* cancelbutton;
	YAPET::Key* key;
	YAPET::PartDec* encentry;

	inline PasswordRecord(const PasswordRecord&) {}
	inline const PasswordRecord& operator=(const PasswordRecord&) { return *this; }

	inline int getWidth() const {
	    return maxX() - 8;
	}

	inline int getHeight() const {
	    return HEIGHT;
	}

	inline int getStartX() const {
	    return maxX()/2 - getWidth()/2;
	}

	inline int getStartY() const {
	    return maxY()/2 - getHeight()/2;
	}
	    
	void createWindow() throw(YAPETUI::UIException);

    public:
	PasswordRecord(YAPET::Key& k, YAPET::PartDec* pe) throw(YAPETUI::UIException);
	~PasswordRecord();

	void run() throw(YAPETUI::UIException);
	void resize() throw(YAPETUI::UIException);
	void refresh() throw(YAPETUI::UIException);
	/**
	 * Caller is responsible for freeing the pointer returned
	 */
	inline YAPET::PartDec* getEncEntry() const { return encentry; }
	inline bool entryChanged() const {
	    return name->isTextChanged() ||
		host->isTextChanged() ||
		username->isTextChanged() ||
		password->isTextChanged() ||
		comment->isTextChanged();
	}
		
};

#endif // _PASSWORDRECORD_H
