// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#ifndef _FILEOPEN_H
#define _FILEOPEN_H

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

#include <secstring.h>
#include <basewindow.h>
#include <listwidget.h>
#include <button.h>
#include <inputwidget.h>

/**
 * @brief Displays a "file open dialog" (sorta)
 *
 * Displays a window where the user can navigate the directory hierarchy and
 * select a file whose ending is \c .pet.
 *
 * @note The filename returned by \c getFilepath() always ends with \c .pet.
 */
class FileOpen : protected YAPETUI::BaseWindow {
    private:
	enum {
	    /**
	     * @brief The max filename length.
	     *
	     * This is only used if we are unable to determine the max length
	     * of the filename by other means.
	     */
	    FALLBACK_PATH_MAX=255
	};

	std::string title;
	WINDOW* window;
	YAPETUI::ListWidget<YAPETUI::secstring>* dir;
	YAPETUI::ListWidget<YAPETUI::secstring>* files;
	YAPETUI::InputWidget* input;
	YAPETUI::Button* okbutton;
	YAPETUI::Button* cancelbutton;

	bool canceled;


	inline FileOpen(const FileOpen&) {}
	inline const FileOpen& operator=(const FileOpen&) { return *this; }

	YAPETUI::secstring directory;
	YAPETUI::secstring filename;

	inline int windowWidth() {
	    return maxX() - 8;
	}

	inline int windowHeight() {
	    return maxY() - 4;
	}

	inline int startX() {
	    return minX() + 4;
	}

	inline int startY() {
	    return minY() + 2;
	}

	void createWindows() throw(YAPETUI::UIException);

	void printTitle() throw(YAPETUI::UIException);

	void printCWD() throw(YAPETUI::UIException);

    protected:
	void getEntries(std::list<YAPETUI::secstring>& d,
			std::list<YAPETUI::secstring>& f)
	    throw(YAPETUI::UIException);

	void getcwd() throw(YAPETUI::UIException);
	void cd(const YAPETUI::secstring d) throw(YAPETUI::UIException);

    public:
	FileOpen(std::string t) throw(YAPETUI::UIException);
	virtual ~FileOpen();

	void run() throw(YAPETUI::UIException);
	void refresh() throw(YAPETUI::UIException);

	void resize() throw(YAPETUI::UIException);

	std::string getFilepath();

	inline bool isCanceled() const { return canceled; }
};

inline
bool endswith(YAPETUI::secstring h, YAPETUI::secstring n) {
    if (n.length() > h.length())
	return false;

    if ( h.substr(h.length()-n.length(),n.length()) == n)
	return true;

    return false;
}

inline
bool endswith(std::string h, std::string n) {
    if (n.length() > h.length())
	return false;

    if ( h.substr(h.length()-n.length(),n.length()) == n)
	return true;

    return false;
}

#endif // _FILEOPEN_H
