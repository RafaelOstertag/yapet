// -*- c++ -*-
//
// $Id$

#ifndef _PASSWORDWIDGET_H
#define _PASSWORDWIDGET_H

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

#include "inputwidget.h"

namespace GPSUI {

    class PasswordWidget : public InputWidget {
	private:
	    inline const PasswordWidget& operator=(const PasswordWidget&) { return *this; }
	    
    public:
	    PasswordWidget(int sx, int sy, int w, int ml = 512) throw(UIException);
	    virtual ~PasswordWidget();
	    
	    virtual void refresh() throw(UIException);
    };

}

#endif // _PASSWORDWIDGET_H
