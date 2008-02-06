// $Id$

#include "messagebox.h"
#include "colors.h"

using namespace GPSUI;

void
MessageBox::createWindow() throw(UIException) {
    if (window != NULL)
	throw UIException("May you consider deleting the window before reallocating");
    if (okbutton != NULL)
	throw UIException("May you consider deleting the button before reallocating");

    window = newwin(BASE_HEIGHT,
		    getWidth(),
		    getStartY(),
		    getStartX());
    if (window == NULL)
	throw UIException("Error creating message window");

    okbutton = new Button("Ok", getStartX() + 1, getStartY() + BASE_HEIGHT -2);
}

MessageBox::MessageBox(std::string t, std::string m) throw(UIException) : window(NULL),
									  okbutton(NULL),
									  title(t),
									  message(m) {
    createWindow();
}

MessageBox::~MessageBox() {
    delete okbutton;
    wclear(window);
    wrefresh(window);
    delwin(window);
}

int
MessageBox::run() throw(UIException) {
    refresh();
    int ch;
    while ( (ch = okbutton->focus()) == KEY_REFRESH )
	Resizeable::refreshAll();
    return ch;
}

void
MessageBox::resize() throw(UIException) {
    delete okbutton;

    int retval = delwin(window);
    if (retval == ERR)
	throw UIException("Error deleting message box");

    okbutton = NULL;
    window = NULL;

    createWindow();
}

void
MessageBox::refresh() throw(UIException) {
    Colors::setcolor(window, MESSAGEBOX);

    int retval = werase(window);
    if (retval == ERR)
	throw UIException("Error erasing window");

    retval = box(window, 0, 0);
    if (retval == ERR)
    throw UIException("Error creating box around message window");

    Colors::setcolor(window, MESSAGEBOX);
    retval = mymvwaddstr(window, 2, 2, message.c_str());
    if (retval == ERR)
    throw UIException("Error printing message");

    // Title
    Colors::setcolor(window, MESSAGEBOX_TITLE);
    retval = mymvwaddstr(window, 0, 2, title.c_str());
    if (retval == ERR)
	throw UIException("Error printing title");

    Colors::setcolor(window, MESSAGEBOX);
    retval = wrefresh(window);
    if (retval == ERR)
	throw UIException("Error refreshing message box");

    okbutton->refresh();
}
