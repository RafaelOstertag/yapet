// -*- c++ -*-
//
// $Id$

#ifndef _SCRDIM_H
#define _SCRDIM_H

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

    class ScrDim  {
	public:
	    inline ScrDim() {}
	    inline virtual ~ScrDim() {}
	    
	    inline int maxX() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_x;
	    }
	    
	    inline int maxY() const {
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		return max_y;
	    }
	    
	    inline int minX() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return x;
	    }
	    
	    inline int minY() const {
		int x, y;
		getbegyx(stdscr, y, x);
		return y;
	    }
	    
    };

}
#endif // _SCRDIM_H
