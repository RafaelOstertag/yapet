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

#include "messagebox.h"
#include "colors.h"

using namespace YAPETUI;

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
    delwin(window);
}

int
MessageBox::run() throw(UIException) {
    refresh();
    int ch;
    while ( (ch = okbutton->focus()) == KEY_REFRESH )
	BaseWindow::refreshAll();
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
