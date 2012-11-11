// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif

#include "../intl.h"
#include "basewindow.h"
#include "inputwidget.h"
#include "colors.h"

using namespace YAPET::UI;

void
InputWidget::moveBackward() throw (UIException) {
    pos--;

    if (pos < 0) {
	pos = 0;
	start_pos--;
    }

    if (start_pos < 0)
	start_pos = 0;

    refresh();
}

void
InputWidget::moveForward() throw (UIException) {
    if ( ( (secstring::size_type) (start_pos + pos + 1) ) > buffer.length() ) {
	refresh();
	return;
    }

    if (pos + 1 > width)
	start_pos++;
    else
	pos++;

    refresh();
}

void
InputWidget::moveHome() throw (UIException) {
    pos = 0;
    start_pos = 0;
    refresh();
}

void
InputWidget::moveEnd() throw (UIException) {
    if (buffer.length() < ( (secstring::size_type) width) ) {
	start_pos = 0;
	pos = buffer.length();
    } else {
	start_pos = buffer.length() - width + 1;
	pos = width - 1;
    }

    refresh();
}

void
InputWidget::hide(bool h) throw (UIException) {
    if ( hidden != h ) {
	hidden = h;
	resize(start_x, start_y, full_width);
    }
    hidden = h;
}

void
InputWidget::processInput (int ch) throw (UIException) {
#ifdef HAVE_ISPRINT
    if (!isprint(ch)) return;
#endif
    if (buffer.length() + 1 > ( (secstring::size_type) max_length) ) return;
    if (readonly) return;

    if ( ( (secstring::size_type) start_pos + pos) > buffer.length() )
	buffer.append ("" + ch);
    else
	buffer.insert (start_pos + pos, 1, ch);

    moveForward();
    // Mark the text as changed
    text_changed = true;
}

void
InputWidget::processBackspace() throw (UIException) {
    if (pos + start_pos == 0) return;
    if (readonly) return;

    moveBackward();
    processDelete();
}

void
InputWidget::processDelete() throw (UIException) {
    if ( ( (secstring::size_type) pos + start_pos) == buffer.length() ) return;
    if (readonly) return;

    buffer.erase (pos + start_pos, 1);

    if ( ( (secstring::size_type) pos + start_pos) > buffer.length() ) {
	if (pos > 0)
	    pos--;
	else
	    start_pos--;
    }

    wdelch(window);
    refresh();
    // Mark the text as changed
    text_changed = true;
}


void
InputWidget::createWindow () throw (UIException) {
    if (window != NULL)
	throw UIException (_ ("May you consider deleting the window first before reallocating it") );

    if ( hidden && readonly &&
	 buffer.length() > 0) {
	width = buffer.length() > full_width ? full_width : buffer.length();
    } else {
	width = full_width;
    }
    window = newwin (1, width, start_y, start_x);

    if (window == NULL)
	throw UIException (_ ("Error creating the input window") );

    int retval;

    if ( hidden && readonly )
	Colors::setcolor (window, INPUTWIDGET_HIDDEN);
    else
	Colors::setcolor (window, INPUTWIDGET_NOFOCUS);

    retval = wclear (window);

    if (retval == ERR)
	throw UIException (_ ("Error clearing input widget") );

#ifdef KEYPAD_RETURN_INT
    retval = keypad (window, TRUE);

    if (retval == ERR)
	throw UIException (_ ("Error setting keypad on input widget") );
#else
    keypad (window, TRUE);
#endif

}

InputWidget::InputWidget (int sx, int sy, int w, int ml, bool ro, bool h)
throw (UIException) : window (NULL),
		      max_length (ml),
		      start_pos (0),
		      pos (0),
		      start_x (sx),
		      start_y (sy),
		      width (w),
		      full_width (w),
		      text_changed (false),
		      readonly (ro),
		      hidden (h) {
    createWindow ();
    hide(hidden);
}

InputWidget::InputWidget (int sx, int sy, int w, bool ro, bool h)
throw (UIException) : window (NULL),
		      max_length (DEFAULT_TEXT_LEN),
		      start_pos (0),
		      pos (0),
		      start_x (sx),
		      start_y (sy),
		      width (w),
		      full_width (w),
		      text_changed (false),
		      readonly (ro),
		      hidden (h){
    createWindow ();
    hide(hidden);
}

InputWidget::~InputWidget() {
    clearText();
    // Commented out because clearText() has already done this.
    // wclear (window);
#ifdef PARANOID
    // To be sure we don't have any sensitive information on the screen and
    // buffers (hopefully)
    wrefresh (window);
#endif
    delwin (window);
}

int
InputWidget::focus() throw (UIException) {
    if ( hidden && readonly )
	Colors::setcolor (window, INPUTWIDGET_HIDDEN);
    else
	Colors::setcolor (window, INPUTWIDGET_FOCUS);

    int retval = wrefresh (window);

    if (retval == ERR)
	throw UIException (_ ("Error refreshing the widget") );

    retval = wmove (window, 0, pos);

    if (retval == ERR)
	throw UIException (_ ("Error moving cursor for widget") );

    visibleCursor (true);
    int ch;

    while (true) {
	ch = wgetch (window);

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
		moveBackward();
		break;
	    case KEY_DOWN:
	    case KEY_RIGHT:
		moveForward();
		break;
	    case KEY_END:
	    case KEY_A1:
		moveEnd();
		break;
	    case KEY_HOME:
	    case KEY_C1:
		moveHome();
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
    if ( hidden && readonly )
	Colors::setcolor (window, INPUTWIDGET_HIDDEN);
    else
	Colors::setcolor (window, INPUTWIDGET_NOFOCUS);

    retval = wrefresh (window);

    if (retval == ERR)
	throw UIException (_ ("Error refreshing the widget") );

    return ch;
}

void
InputWidget::refresh() throw (UIException) {
    int retval;

    if (buffer.length() > 0) {
	secstring sub = buffer.substr (start_pos, width);

	// We don't ask for the return value because it always returns an error
	// if we reach the end of line and this would break the hidden
	// display. If somebody has a better approach, let me know.
	mymvwaddnstr (window,
		      0,
		      0,
		      sub.c_str(),
		      width);

	if (pos >= width - 1 )
	    retval = wmove (window, 0, width - 1);
	else
	    retval = wmove (window, 0, pos);

	if (retval == ERR)
	    throw UIException (_ ("Error moving cursor") );
    }

    retval = wrefresh (window);

    if (retval == ERR)
	throw UIException (_ ("Error refreshing input widget") );
}

void
InputWidget::resize (int sx, int sy, int w) throw (UIException) {
    int retval = wclear (window);

    if (retval == ERR)
	throw UIException (_ ("Error clearing input widget") );

    retval = wrefresh (window);

    if (retval == ERR)
	throw UIException (_ ("Error refreshing input widget") );

    retval = delwin (window);

    if (retval == ERR)
	throw UIException (_ ("Error deleting input widget") );

    window = NULL;
    start_x = sx;
    start_y = sy;
    width = full_width = w;
    createWindow ();
}

void
InputWidget::setText (secstring t) throw (UIException) {
    clearText();
    buffer = t;
    start_pos = 0;
    pos = 0;
    text_changed = false;
    hide(hidden);
    refresh();
}

void
InputWidget::clearText() {
    for (secstring::size_type i = 0; i < buffer.length(); i++)
	buffer[i] = 0;

    buffer.clear();
    wclear (window);
}

void
InputWidget::setReadonly(bool ro) {
    readonly = ro;
    // Hidden is coupled with readonly, so we call hide
    hide(hidden);

    refresh();
}

void
InputWidget::setHidden(bool h) {
    hide(h);
}
