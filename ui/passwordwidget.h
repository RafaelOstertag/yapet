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

#ifndef _PASSWORDWIDGET_H
#define _PASSWORDWIDGET_H

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

#include "inputwidget.h"

namespace YAPETUI {

    /**
     * @brief A password input widget.
     *
     * Basically the same as \c InputWidget, but shows asterisks \c '*' instead
     * of the characters typed by the user.
     */
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
