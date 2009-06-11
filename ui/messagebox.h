// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

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

#include "uiexception.h"
#include "basewindow.h"
#include "button.h"

namespace YAPETUI {
    /**
     * @brief Displays a message in a window on the screen.
     *
     * Displays a message in a separate window on the screen. A button will be
     * available to quit the message.
     *
     * Line breaks in the message string are not handled at all. So try to
     * avoid them.
     */
    class MessageBox : protected BaseWindow {
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
