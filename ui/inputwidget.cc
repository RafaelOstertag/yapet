// $Id$

#include "resizeable.h"
#include "inputwidget.h"
#include "colors.h"

using namespace GPSUI;

void
InputWidget::moveBackward() throw(UIException) {
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
InputWidget::moveForward() throw(UIException) {
    if ( ((secstring::size_type)(start_pos + pos + 1)) > buffer.length()) {
	refresh();
	return;
    }

    if (pos+1 > width)
	start_pos++;
    else
	pos++;

    refresh();
}

void
InputWidget::moveHome() throw(UIException) {
    pos = 0;
    start_pos = 0;
    refresh();
}

void
InputWidget::moveEnd() throw(UIException) {
    if (buffer.length() < ((secstring::size_type)width) ) {
	start_pos = 0;
	pos = buffer.length();
    } else {
	start_pos = buffer.length() - width + 1;
	pos = width - 1;
    }
    refresh();
}


void
InputWidget::processInput(int ch) throw(UIException) {
    if (buffer.length()+1 > ((secstring::size_type)max_length)) return;

    if ( ((secstring::size_type)start_pos + pos) > buffer.length()) 
	buffer.append(""+ch);
    else
	buffer.insert(start_pos+pos, 1, ch);

    moveForward();

    // Mark the text as changed
    text_changed = true;
}

void
InputWidget::processBackspace() throw(UIException) {
    if (pos + start_pos == 0) return;

    moveBackward();
    processDelete();
}

void
InputWidget::processDelete() throw(UIException) {
    if ( ((secstring::size_type)pos + start_pos) == buffer.length()) return;

    buffer.erase(pos + start_pos, 1);
    if ( ((secstring::size_type)pos + start_pos) > buffer.length()) {
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
InputWidget::createWindow(int sx, int sy, int w) throw(UIException) {
    if (window != NULL)
	throw UIException("May you consider deleting the window first before reallocating it");

    window = newwin(1, w, sy, sx);
    if (window == NULL)
	throw UIException("Error creating the input window");

    Colors::setcolor(window, INPUTWIDGET_NOFOCUS);

    int retval = wclear(window);
    if (retval == ERR)
	throw UIException("Error clearing input widget");

    retval = keypad(window, TRUE);
    if (retval == ERR)
	throw UIException("Error setting keypad on input widget");

    //refresh();
}

InputWidget::InputWidget(int sx, int sy, int w, int ml) 
    throw(UIException) : window(NULL),
			 max_length(ml),
			 start_pos(0),
			 pos(0),
			 width(w),
			 text_changed(false) {
    createWindow(sx, sy, w);
}

InputWidget::~InputWidget() {
    clearText();
    wrefresh(window);
    delwin(window);
}

int
InputWidget::focus() throw(UIException) {
    Colors::setcolor(window, INPUTWIDGET_FOCUS);

    int retval = wrefresh(window);
    if (retval == ERR)
	throw UIException("Error refreshing the widget");

    retval = wmove(window, 0, pos);
    if (retval == ERR)
	throw UIException("Error moving cursor for widget");

    curs_set(2);
    int ch;
    while ( (ch=wgetch(window)) != '\n' && ch != '\t') {
	switch (ch) {
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
	    ungetch('\n');
	    break;
	case KEY_DC:
        case 127:
	    processDelete();
	    break;
	case KEY_BACKSPACE:
        case 8:
	    processBackspace();
	    break;
#ifdef HAVE_WRESIZE
	case KEY_RESIZE:
	    goto BAILOUT;
	    break;
#endif // HAVE_WRESIZE
	case KEY_REFRESH:
	    Resizeable::refreshAll();
	    break;
	default:
	    processInput(ch);
	    break;
	}
    }
 BAILOUT:
    curs_set(0);

    Colors::setcolor(window, INPUTWIDGET_NOFOCUS);

    retval = wrefresh(window);
    if (retval == ERR)
	throw UIException("Error refreshing the widget");
    return ch;
}

void
InputWidget::refresh() throw(UIException) {
    int retval = werase(window);
    if (retval == ERR)
	throw UIException("Error clearing input widget");

    if (buffer.length() > 0) {
	secstring sub = buffer.substr(start_pos, width);
	retval = mymvwaddnstr(window,
			      0,
			      0,
			      sub.c_str(),
			      width-1);
	if (retval == ERR)
	    throw UIException("Error adding text to window");

	if (pos >= width - 1)
	    retval = wmove(window, 0, width-1);
	else
	    retval = wmove(window, 0, pos);

	if (retval == ERR)
	    throw UIException("Error moving cursor");
    }

    retval = wrefresh(window);
    if (retval == ERR)
	throw UIException("Error refreshing input widget");

}

void
InputWidget::resize(int sx, int sy, int w) throw(UIException) {
    int retval = wclear(window);
    if (retval == ERR)
	throw UIException("Error clearing input widget");

    retval = wrefresh(window);
    if (retval == ERR)
	throw UIException("Error refreshing input widget");

    retval = delwin(window);
    if (retval == ERR)
	throw UIException("Error deleting input widget");

    window = NULL;
    createWindow(sx, sy, w);
}

void
InputWidget::setText(secstring t) throw(UIException) {
    clearText();
    buffer = t;
    start_pos = 0;
    pos = 0;
    text_changed = false;
    refresh();
}

void
InputWidget::clearText() {
    for(secstring::size_type i=0; i < buffer.length(); i++)
	buffer[i]=0;

    buffer.clear();
	wclear(window);
}
