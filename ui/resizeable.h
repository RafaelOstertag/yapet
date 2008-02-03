// -*- c++ -*-
//
// $Id$

#ifndef _RESIZEABLE_H
#define _RESIZEABLE_H

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

#include <list>

namespace GPSUI {

    class Resizeable {
	private:
	    static std::list<Resizeable*> resizeable_list;
	    
	protected:
	    static void registerResizeable(Resizeable* r);
	    static void unregisterResizeable(Resizeable* r);
	    
	public:
	    static void resizeAll();
	    static void refreshAll();
	    Resizeable();
	    virtual ~Resizeable();
	    virtual void resize() = 0;
	    virtual void refresh() = 0;
    };
    
}

#endif // _RESIZEABLE_H

