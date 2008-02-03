// -*- c++ -*-
//
// $Id$

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
#include <scrdim.h>

class MainWindow : protected GPSUI::Resizeable,
		   protected GPSUI::ScrDim {

    private:
	WINDOW* toprightwin;
	WINDOW* bottomrightwin;
	GPSUI::ListWidget<GPSAFE::PartDec>* recordlist;
	StatusBar statusbar;
	bool records_changed;

	GPSAFE::Key* key;
	GPSAFE::File* file;

	inline MainWindow(const MainWindow&) {}
	inline const MainWindow& operator=(const MainWindow&) { return *this;}

    protected:
	void printTitle() throw (GPSUI::UIException);

	void topRightWinContent() throw(GPSUI::UIException);

	void bottomRightWinContent() throw(GPSUI::UIException);

	void createWindow() throw(GPSUI::UIException);

	void refresh() throw(GPSUI::UIException);

	void createFile(std::string& filename) throw(GPSUI::UIException);
	void openFile(std::string filename) throw(GPSUI::UIException);
	void saveFile();
	void closeFile();

	void addNewRecord();
	void editSelectedRecord();
	void deleteSelectedRecord() throw(GPSUI::UIException);
	bool quit();
	void lockScreen() const throw(GPSUI::UIException);

    public:
	MainWindow() throw(GPSUI::UIException);
	virtual ~MainWindow();

	void run() throw(GPSUI::UIException);
	void resize() throw(GPSUI::UIException);
};

#endif // _MAINWINDOW_H
