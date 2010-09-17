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

#include "../intl.h"
#include "passwordwidget.h"
#include "basewindow.h"
#include "colors.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

using namespace YAPET::UI;

PasswordWidget::PasswordWidget (int sx, int sy, int w, int ml)
throw (UIException) : InputWidget (sx, sy, w, ml) {}

PasswordWidget::~PasswordWidget() {
}

int
PasswordWidget::focus() throw (UIException) {
    Colors::setcolor (getWindow(), INPUTWIDGET_FOCUS);
    int retval = wrefresh (getWindow());

    if (retval == ERR)
        throw UIException (_ ("Error refreshing the widget") );

    retval = wmove (getWindow(), 0, getPos());

    if (retval == ERR)
        throw UIException (_ ("Error moving cursor for widget") );

    visibleCursor (true);
    int ch;

    while (true) {
        ch = wgetch (getWindow());

        switch (ch) {
                // Bailout keys
#ifdef HAVE_WRESIZE
	    case KEY_RESIZE:
#endif // HAVE_WRESIZE
            case '\n':
            case KEY_TAB:
            case KEY_ESC:
	    case KEY_CTRL_E:
                goto BAILOUT;
                // Motion and other keys
            case KEY_UP:
            case KEY_LEFT:
	    case KEY_DOWN:
            case KEY_RIGHT:
	    case KEY_END:
            case KEY_A1:
	    case KEY_HOME:
            case KEY_C1:
                break;
            case KEY_ENTER:
                ungetch ('\n');
                break;
            case KEY_DC:
                processDelete();
                break;
            case KEY_BACKSPACE:
            case 127:
                processBackspace();
                break;
            case KEY_REFRESH:
                BaseWindow::refreshAll();
                break;
            default:
                processInput (ch);
                break;
        }
    }

BAILOUT:
    visibleCursor (false);
    Colors::setcolor (getWindow(), INPUTWIDGET_NOFOCUS);
    retval = wrefresh (getWindow());

    if (retval == ERR)
        throw UIException (_ ("Error refreshing the widget") );

    return ch;
}

void
PasswordWidget::refresh() throw (UIException) {
    int retval = wclear (getWindow() );

    if (retval == ERR)
        throw UIException (_ ("Error clearing input widget") );

    if (getPos() > 0) {
        char* tmp = (char*) malloc (getPos() + 1);
        memset (tmp, '*', getPos() );
        tmp[getPos() ] = '\0';
        retval = mymvwaddnstr (getWindow(),
                               0,
                               0,
                               tmp,
                               getWidth() - 1);
        free (tmp);

        if (retval == ERR)
            throw UIException (_ ("Error adding text to window") );

        if (getPos() >= getWidth() - 1)
            retval = wmove (getWindow(), 0, getWidth() - 1);
        else
            retval = wmove (getWindow(), 0, getPos() );

        if (retval == ERR)
            throw UIException (_ ("Error moving cursor") );
    }

    retval = wrefresh (getWindow() );

    if (retval == ERR)
        throw UIException (_ ("Error refreshing input widget") );
}
