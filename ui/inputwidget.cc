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
InputWidget::processInput (int ch) throw (UIException) {
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

    refresh();
    // Mark the text as changed
    text_changed = true;
}


void
InputWidget::createWindow (int sx, int sy, int w) throw (UIException) {
    if (window != NULL)
        throw UIException (_ ("May you consider deleting the window first before reallocating it") );

    window = newwin (1, w, sy, sx);

    if (window == NULL)
        throw UIException (_ ("Error creating the input window") );

    Colors::setcolor (window, INPUTWIDGET_NOFOCUS);
    int retval = wclear (window);

    if (retval == ERR)
        throw UIException (_ ("Error clearing input widget") );

    retval = keypad (window, TRUE);

    if (retval == ERR)
        throw UIException (_ ("Error setting keypad on input widget") );

    //refresh();
}

InputWidget::InputWidget (int sx, int sy, int w, int ml, bool ro)
throw (UIException) : window (NULL),
		      max_length (ml),
		      start_pos (0),
		      pos (0),
		      width (w),
		      readonly (ro),
		      text_changed (false) {
    createWindow (sx, sy, w);
}

InputWidget::~InputWidget() {
    clearText();
    // Commented out because clearText() has already done this.
    // wclear (window);

    // To be sure we don't have any sensitive information on the screen and
    // buffers (hopefully)
    wrefresh (window);
    delwin (window);
}

int
InputWidget::focus() throw (UIException) {
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
    Colors::setcolor (window, INPUTWIDGET_NOFOCUS);
    retval = wrefresh (window);

    if (retval == ERR)
        throw UIException (_ ("Error refreshing the widget") );

    return ch;
}

void
InputWidget::refresh() throw (UIException) {
    int retval = werase (window);

    if (retval == ERR)
        throw UIException (_ ("Error clearing input widget") );

    if (buffer.length() > 0) {
        secstring sub = buffer.substr (start_pos, width);
        retval = mymvwaddnstr (window,
                               0,
                               0,
                               sub.c_str(),
                               width - 1);

        if (retval == ERR)
            throw UIException (_ ("Error adding text to window") );

        if (pos >= width - 1)
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
    createWindow (sx, sy, w);
}

void
InputWidget::setText (secstring t) throw (UIException) {
    clearText();
    buffer = t;
    start_pos = 0;
    pos = 0;
    text_changed = false;
    refresh();
}

void
InputWidget::clearText() {
    for (secstring::size_type i = 0; i < buffer.length(); i++)
        buffer[i] = 0;

    buffer.clear();
    wclear (window);
}
