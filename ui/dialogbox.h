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

#ifndef _DIALOGBOX_H
#define _DIALOGBOX_H

#ifdef HAVE_CONFIG_H
# include <config.h>
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

#ifdef HAVE_STRING
# include <string>
#endif

#include "messagebox.h"

namespace YAPETUI {

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
    
