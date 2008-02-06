// $Id$
//
// @@REPLACE@@
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

#include "button.h"
#include "colors.h"
#include "resizeable.h"

using namespace GPSUI;

void
Button::createWindow() throw(UIException) {
    window = newwin (1, BASE_SIZE + label.length(), start_y, start_x);
    if (window == NULL)
        throw UIException ("Error creating button");

    //refresh();
}

Button::Button (std::string l, int x, int y) : window (NULL),
        label (l),
        start_x (x),
        start_y (y) {
    createWindow();
}

Button::~Button() {
    wclear(window);
    wrefresh(window);
    delwin (window);
}


void
Button::setLabel (std::string l) throw (UIException) {
    label = l;
    int retval = wclear (window);
    if (retval == ERR)
        throw UIException ("Error clearing button");
    retval = wrefresh (window);
    if (retval == ERR)
        throw UIException ("Error refreshing button");
    retval = delwin (window);
    if (retval == ERR)
        throw UIException ("Error deleting button");


}

void
Button::refresh() throw (UIException) {
    Colors::setcolor(window, BUTTON_NOFOCUS);
    int retval = werase(window);
    if (retval == ERR)
        throw UIException ("Error erasing button");

    mvwprintw (window, 0, 0, "[ %s ]", label.c_str());

    retval = wrefresh (window);
    if (retval == ERR)
        throw UIException ("Error refreshing button");
}

int
Button::focus() throw (UIException) {
    Colors::setcolor(window, BUTTON_FOCUS);
    mvwprintw (window, 0, 0, "[ %s ]", label.c_str());

    int retval = touchwin (window);
    if (retval == ERR)
        throw UIException ("Erro touching window");

    retval = wrefresh (window);
    if (retval == ERR)
        throw UIException ("Error refreshing button");

    retval = keypad (window, TRUE);
    if (retval == ERR)
        throw UIException ("Error setting keypad");

    int ch;
    while (true) {
        ch = wgetch (window);

        switch (ch) {
        case '\n':
        case KEY_ENTER:
	    ch = '\n';
            onClick();
	    goto BAILOUT;
            break;
        case '\t':
        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
	    ch = '\t';
            goto BAILOUT;
            break;
	case KEY_REFRESH:
	    Resizeable::refreshAll();
	    break;
#ifdef HAVE_WRESIZE
	case KEY_RESIZE:
	    goto BAILOUT;
	    break;
#endif // HAVE_WRESIZE
        }
    }

 BAILOUT:
    Colors::setcolor(window, BUTTON_NOFOCUS);

    mvwprintw (window, 0, 0, "[ %s ]", label.c_str());


    retval = touchwin (window);
    if (retval == ERR)
        throw UIException ("Erro touching window");

    retval = wrefresh (window);
    if (retval == ERR)
        throw UIException ("Error refreshing button");

    return ch;
}
