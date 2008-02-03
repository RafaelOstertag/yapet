// $Id$

#include "button.h"
#include "colors.h"

using namespace GPSUI;

void
Button::show() throw(UIException) {
    window = newwin (1, BASE_SIZE + label.length(), start_y, start_x);
    if (window == NULL)
        throw UIException ("Error creating button");

    refresh();
}

Button::Button (std::string l, int x, int y) : window (NULL),
        label (l),
        start_x (x),
        start_y (y) {
    show();
}

Button::~Button() {
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
    bool stay_in_loop = true;
    while (stay_in_loop) {
        ch = wgetch (window);

        switch (ch) {
        case '\n':
        case KEY_ENTER:
	    ch = '\n';
            onClick();
            stay_in_loop = false;
            break;
        case '\t':
        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_UP:
        case KEY_DOWN:
            stay_in_loop = false;
            break;
        }
    }

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
