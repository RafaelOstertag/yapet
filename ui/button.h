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

#ifndef _BUTTON_H
#define _BUTTON_H

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
#include "listwidget.h"

/**
 * @brief Holds primitive classes associated with the user interface.
 *
 * This namepsace holds the primitive classes used to build the user
 * interface of yapet.
 */
namespace YAPETUI {
    /**
     * @brief A button.
     *
     * Displays a button.
     *
     * When the user presses the button, \c focus() returns \c \\n.
     */
    class Button {
	private:
	    enum {
		/**
		 * @brief The base size of a button.
		 *
		 * A button with no label would look like this \c [  ].
		 */
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
	    void createWindow() throw(UIException);
	    
	public:
	    Button(std::string l, int x, int y);
	    virtual ~Button();
	    
	    void setLabel(std::string l) throw(UIException);
	    std::string getLabel() { return label; }
	    
	    void refresh() throw(UIException);
	    
	    /**
	     * @brief Shows the button and waits for input.
	     *
	     * Shows the button and waits for input. When button is
	     * pressed, it returns \c \\n.
	     *
	     * @return on press \c \\n.
	     */
	    int focus() throw(UIException);
	    /**
	     *
	     */
	    inline int getLength() const {
		return BASE_SIZE + label.length();
	    }
    };
}

#endif // _BUTTON_H
