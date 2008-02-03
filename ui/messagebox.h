// -*- c++ -*-
//
// $Id$

#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

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
#include "resizeable.h"
#include "button.h"
#include "scrdim.h"

namespace GPSUI {

    class MessageBox : protected Resizeable, protected ScrDim {
	private:
	    enum {
		BASE_HEIGHT=6,
		BASE_WIDTH=4
	    };
	    WINDOW* window;
	    Button* okbutton;
	    
	    std::string title;
	    std::string message;
	    
	    void createWindow() throw(UIException);
	    
	protected:
	    inline int getBaseHeight() const { return BASE_HEIGHT; }
	    
	    inline int getWidth() const {
		return BASE_WIDTH + message.length();
	    }
	    
	    inline int getStartX() const {
		return maxX()/2 - getWidth()/2;
	    }
	    
	    inline int getStartY() const {
		return maxY()/2 - BASE_HEIGHT/2;
	    }
	    
	    inline int getOkButtonLength() const {
		if (okbutton == NULL) return -1;
		return okbutton->getLength();
	    }
	    
	public:
	    MessageBox(std::string t, std::string m) throw(UIException);
	    virtual ~MessageBox();
	    
	    virtual int run() throw(UIException) ;
	    virtual void resize() throw(UIException);
	    virtual void refresh() throw(UIException);
    };
    
}

#endif // _MESSAGEBOX_H
    
