// -*- c++ -*-
//
// $Id$

#ifndef _STATUSBAR_H
#define _STATUSBAR_H

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

#include <string>

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
