// -*- c++ -*-
//
// $Id$

#ifndef _DIALOGBOX_H
#define _DIALOGBOX_H

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

#include "messagebox.h"

namespace GPSUI {

    enum ANSWER {
	ANSWER_OK,
	ANSWER_CANCEL
    };
    
    class DialogBox : public MessageBox {
	private:
	    Button* cancelbutton;
	    ANSWER answer;
	    
	public:
	    DialogBox(std::string t, std::string m) throw(UIException);
	    virtual ~DialogBox();
	    
	    virtual int run() throw(UIException) ;
	    virtual void resize() throw(UIException);
	    virtual void refresh() throw(UIException);
	    ANSWER getAnswer() const { return answer; }
    };
    
}

#endif // _DIALOGBOX_H
    
