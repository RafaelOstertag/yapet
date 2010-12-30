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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#include "../intl.h"
#include "checkboxgroup.h"
#include "colors.h"
// BaseWindow::refreshAll
#include "basewindow.h"

using namespace YAPET::UI;

inline int
CheckBoxGroup::ItemsUsableWidth() const {
    return width - BASE_WIDTH;
}

inline int
CheckBoxGroup::ItemsUsableHeight() const {
    return height - BASE_HEIGHT;
}

inline int
CheckBoxGroup::lastItemPos() const {
    int max_pos = items.size() > (std::vector<std::string>::size_type) ItemsUsableHeight() ?
                  ItemsUsableHeight() : items.size();
    return max_pos > MAX_ITEMS ? MAX_ITEMS : max_pos;
}

void
CheckBoxGroup::addCheckBox (int pos) throw (UIException) {
    assert ( (pos > -1) &&  (pos <= lastItemPos() ) );
    int retval = mymvwaddstr (window,
                              1 + pos,
                              2,
                              "[ ]");

    if (retval == ERR)
        throw UIException (_ ("Error printing check box") );
}

void
CheckBoxGroup::setCheckMark (int pos, bool mark) throw (UIException) {
    assert ( (pos > -1) &&  (pos <= lastItemPos() ) );
    int retval = mymvwaddstr (window,
                              1 + pos,
                              3,
                              (mark ? "X" : " ") );

    if (retval == ERR)
        throw UIException (_ ("Error printing check box") );
}

void
CheckBoxGroup::setCheckMark (int pos)  throw (UIException) {
    assert ( (pos > -1) &&  (pos <= lastItemPos() ) );
    setCheckMark (pos, ( options & (1 << pos) ? true : false) );
}

void
CheckBoxGroup::setCursor (int pos) throw (UIException) {
    assert ( (pos > -1) &&  (pos <= lastItemPos() ) );
    int retval = wmove (window, 1 + pos, 3);

    if (retval == ERR)
        throw UIException (_ ("Error moving cursor for widget") );
}

void
CheckBoxGroup::displayItems() throw (UIException) {
    for (int i = 0; i < lastItemPos(); i++) {
        addCheckBox (i);
        setCheckMark (i);
        int retval = mymvwaddnstr (window,
                                   1 + i,
                                   6,
                                   items[i].c_str(),
                                   ItemsUsableWidth() );

        if (retval == ERR)
            throw UIException (_ ("Error printing check box item") );
    }
}

void
CheckBoxGroup::createWindow() throw (UIException) {
    window = newwin (height, width, start_y, start_x);

    if (window == NULL)
        throw UIException (_ ("Error creating checkbox window") );

    Colors::setcolor (window, CHECKBOXGROUP);
#ifdef KEYPAD_RETURN_INT
    int retval = keypad (window, true);
    if (retval == ERR)
        throw UIException (_ ("Error enabling keypad") );
#else
    keypad (window, true);
#endif
}

CheckBoxGroup::CheckBoxGroup (std::string t,
                              const std::vector<std::string>& it,
                              uint16_t o,
                              int x,
                              int y,
                              int w,
                              int h) throw (UIException) : title (t),
        items (it),
        options (o),
        start_x (x),
        start_y (y),
        width (w),
        height (h) {
    // Copy/Paste from listwidget.h. Don't ask me why this is checked...
    if ( x == -1 ||
            y == -1 ||
            width == -1 ||
            height == -1 )
        throw UIException (_ ("No idea of the dimension of the widget") );

    createWindow();
}

CheckBoxGroup::~CheckBoxGroup() {
    assert (window != NULL);
    delwin (window);
}

void
CheckBoxGroup::refresh() throw (UIException) {
    Colors::setcolor (window, CHECKBOXGROUP);
    int retval = werase (window);

    if (retval == ERR)
        throw UIException (_ ("Error erasing window") );

    retval = box (window, 0, 0);

    if (retval == ERR)
        throw UIException (_ ("Error setting border") );

    Colors::setcolor (window, CHECKBOXGROUP_TITLE);
    retval = mymvwaddstr (window,
                          0,
                          2,
                          title.c_str() );

    if (retval == ERR)
        throw UIException (_ ("Error printing title") );

    Colors::setcolor (window, MESSAGEBOX);
    displayItems();
    retval = wrefresh (window);

    if (retval == ERR)
        throw UIException (_ ("Error refreshing message box") );
}

/**
 *
 * Focus the widget.
 *
 * KEY_UP, 'k', KEY_DOWN, 'j', KEY_TAB are selection movement
 * keys. KEY_ESC, KEY_RESIZE will make it loose focus. KEY_ENTER,
 * KEY_SPACE will mark/unmark check box.
 *
 * @return the key that made the widget loose the focus, either KEY_TAB, KEY_RESIZE, or KEY_ESC
 */
int
CheckBoxGroup::focus() throw (UIException) {
    refresh();
    int cur_pos = 0;
    visibleCursor (true);
    int ch = 0;
    int retval = 0;

    while (1) {
        retval = wmove (window, 1 + cur_pos, 3);

        if (retval == ERR)
            throw UIException (_ ("Error moving cursor for widget") );

        ch = wgetch (window);

        switch (ch) {
                // Bailout keys
#ifdef HAVE_WRESIZE
            case KEY_RESIZE:
#endif // HAVE_WRESIZE
            case KEY_ESC:
            case KEY_TAB:
                goto BAILOUT;
            case KEY_UP:
            case 'k': // vi key

                if ( cur_pos == 0 ) {
                    // Wrap around
                    cur_pos = lastItemPos();
                } else {
                    cur_pos --;
                }

                break;
            case KEY_DOWN:
            case 'j': // vi key

                if ( cur_pos < lastItemPos() - 1  ) {
                    // Wrap around
                    cur_pos ++;
                } else {
                    cur_pos = 0;
                }

                break;
            case KEY_ENTER:
            case KEY_SPACE:
            case '\n':
                options ^= (1 << cur_pos);
                setCheckMark (cur_pos);
                break;
            case KEY_REFRESH:
                BaseWindow::refreshAll();
                break;
            default:
                break;
        }
    }

BAILOUT:
    visibleCursor (false);
    return ch;
}

void
CheckBoxGroup::resize (int sx, int sy, int w, int h) throw (UIException) {
    int retval = delwin (window);

    if (retval == ERR)
        throw UIException (_ ("Error deleting message box") );

    start_x = sx;
    start_y = sy;
    width = w;
    height = h;
    createWindow();
}
