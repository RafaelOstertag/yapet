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
#include <resizeable.h>
#include <listwidget.h>
#include <button.h>
#include <inputwidget.h>

class FileOpen : protected GPSUI::Resizeable {
    private:
	enum {
	    FALLBACK_PATH_MAX=255
	};

	std::string title;
	WINDOW* window;
	GPSUI::ListWidget<GPSUI::secstring>* dir;
	GPSUI::ListWidget<GPSUI::secstring>* files;
	GPSUI::InputWidget* input;
	GPSUI::Button* okbutton;
	GPSUI::Button* cancelbutton;
	
	bool canceled;


	inline FileOpen(const FileOpen&) {}
	inline const FileOpen& operator=(const FileOpen&) { return *this; }

	GPSUI::secstring directory;
	GPSUI::secstring filename;

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

	void createWindows() throw(GPSUI::UIException);

	void printTitle() throw(GPSUI::UIException);

	void printCWD() throw(GPSUI::UIException);

    protected:
	void getEntries(std::list<GPSUI::secstring>& d, std::list<GPSUI::secstring>& f) throw(GPSUI::UIException);
	void getcwd() throw(GPSUI::UIException);
	void cd(const GPSUI::secstring d) throw(GPSUI::UIException);

    public:
	FileOpen(std::string t) throw(GPSUI::UIException);
	virtual ~FileOpen();

	void run() throw(GPSUI::UIException);
	void refresh() throw(GPSUI::UIException);

	void resize() throw(GPSUI::UIException);
	
	std::string getFilepath();

	inline bool isCanceled() const { return canceled; }
};

#endif // _FILEOPEN_H
