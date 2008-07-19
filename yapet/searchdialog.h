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

#ifndef _SEARCHDIALOG_H
#define _SEARCHDIALOG_H

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

#include <basewindow.h>
#include <button.h>
#include <inputwidget.h>

/**
 * @brief Search dialog
 *
 * Dialog for entering the search term
 */
class SearchDialog : protected YAPETUI::BaseWindow {
    private:
	enum {
	    HEIGHT = 6
	};

	WINDOW* window;
	YAPETUI::InputWidget* searchtermw;
	YAPETUI::Button* okbutton;
	YAPETUI::Button* cancelbutton;
	YAPETUI::secstring searchterm;

	bool canceled;

	inline SearchDialog(const SearchDialog&) {}
	inline const SearchDialog& operator=(const SearchDialog&) {
	    return *this; }

	inline int getWidth() const { return maxX() - 8; }
	inline int getHeight() const { return HEIGHT; }

	inline int getStartX() const { return maxX()/2 - getWidth()/2; }
	inline int getStartY() const { return maxY()/2 - getHeight()/2; }

	void createWindow() throw(YAPETUI::UIException);

    public:
	SearchDialog() throw (YAPETUI::UIException);
	~SearchDialog();

	void run() throw(YAPETUI::UIException);
	void resize() throw(YAPETUI::UIException);
	void refresh() throw(YAPETUI::UIException);

	inline bool isCanceled() const { return canceled; }
	inline const char* getSearchTerm() const {
	    return searchtermw->getText().c_str();
	}
};

#endif // _SEARCHDIALOG_H
