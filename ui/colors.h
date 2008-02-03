// -*- c++ -*-
//
// $Id$

#ifndef _COLORS_H
#define _COLORS_H

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

namespace GPSUI {
    
    enum COLORS {
	DEFAULT = 0,
	MESSAGEBOX_TITLE,
	MESSAGEBOX,
	INPUTWIDGET_NOFOCUS,
	INPUTWIDGET_FOCUS,
	BUTTON_NOFOCUS,
	BUTTON_FOCUS,
	LISTWIDGET
    };

    /**
     * Struct holding the color
     */
    struct color {
	    /** The number of the pair
	     * 
	     * The number of the pair
	     */
	    short no;
	    /**
	     * The foreground color
	     */
	    short fg;
	    /**
	     * The background color
	     */
	    short bg;
	    /**
	     * The attribute used when no color is available
	     */
	    int attr;
    };

    class Colors {
	private:
	    static bool initialized;
	    static color colors[];
	public:
	    static void initColors();
	    static void setcolor(WINDOW* w, COLORS c);
	    static void unsetcolor(WINDOW* w, COLORS c);
	    static short getcolor(COLORS c);
    };

    
}

#endif // _COLORS_H
