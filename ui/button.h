// -*- c++ -*-
//
// $Id$

#ifndef _BUTTON_H
#define _BUTTON_H

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

#include "uiexception.h"
#include "listwidget.h"

namespace GPSUI {
    class Button {
	private:
	    enum {
		BASE_SIZE = 4
	    };
	    
	    WINDOW* window;
	    
	    inline Button(const Button&) {}
	    inline const Button& operator=(const Button&) { return *this; }
	    
	    std::string label;
	    
	    int start_x;
	    int start_y;
	    
	protected:
	    inline virtual void onClick() {};
	    void show() throw(UIException);
	    
	public:
	    Button(std::string l, int x, int y);
	    virtual ~Button();
	    
	    void setLabel(std::string l) throw(UIException);
	    std::string getLabel() { return label; }
	    
	    void refresh() throw(UIException);
	    
	    int focus() throw(UIException);
	    inline int getLength() const { return BASE_SIZE + label.length(); }
    };
}

#endif // _BUTTON_H
