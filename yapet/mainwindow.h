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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H


#ifdef HAVE_CONFIG_H
#include <config.h>
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

#include <file.h>
#include <resizeable.h>
#include <uiexception.h>
#include <statusbar.h>
#include <listwidget.h>

class MainWindow : protected YAPETUI::Resizeable {
    private:
	WINDOW* toprightwin;
	WINDOW* bottomrightwin;
	YAPETUI::ListWidget<YAPET::PartDec>* recordlist;
	StatusBar statusbar;
	bool records_changed;

	YAPET::Key* key;
	YAPET::File* file;

	inline MainWindow(const MainWindow&) {}
	inline const MainWindow& operator=(const MainWindow&) { return *this;}

    protected:
	void printTitle() throw (YAPETUI::UIException);

	void topRightWinContent() throw(YAPETUI::UIException);

	void bottomRightWinContent() throw(YAPETUI::UIException);

	void createWindow() throw(YAPETUI::UIException);

	void refresh() throw(YAPETUI::UIException);

	void createFile(std::string& filename) throw(YAPETUI::UIException);
	void openFile(std::string filename) throw(YAPETUI::UIException);
	void saveFile();
	void closeFile();

	void addNewRecord();
	void editSelectedRecord();
	void deleteSelectedRecord() throw(YAPETUI::UIException);
	bool quit();
	void lockScreen() const throw(YAPETUI::UIException);

    public:
	MainWindow() throw(YAPETUI::UIException);
	virtual ~MainWindow();

	void run() throw(YAPETUI::UIException);
	void run(std::string fn);
	void resize() throw(YAPETUI::UIException);
};

#endif // _MAINWINDOW_H
