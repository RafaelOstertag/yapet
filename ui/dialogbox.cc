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

#include "../intl.h"
#include "dialogbox.h"

using namespace YAPETUI;

DialogBox::DialogBox(std::string t, std::string m)
    throw(UIException) : MessageBox(t, m),
			 cancelbutton(NULL),
			 answer(ANSWER_CANCEL) {
    cancelbutton = new Button(_("Cancel"),
			      getStartX() + 2 + getOkButtonLength(),
			      getStartY() + getBaseHeight() -2);
}

DialogBox::~DialogBox() {
    delete cancelbutton;
}

int
DialogBox::run() throw(UIException) {
    refresh();
    while (true) {
#ifdef HAVE_WRESIZE
	int ch;
	while ( (ch = MessageBox::run()) == KEY_RESIZE )
	    BaseWindow::resizeAll();
#else // HAVE_RESIZE
	int ch = MessageBox::run();
#endif // HAVE_RESIZE
	if (ch == '\n') {
	    answer = ANSWER_OK;
	    return ch;
	}

#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE )
	    BaseWindow::resizeAll();
#else // HAVE_RESIZE
	ch = cancelbutton->focus();
#endif // HAVE_RESIZE
	if (ch == '\n') {
	    answer = ANSWER_CANCEL;
	    return ch;
	}
    }
}

void
DialogBox::resize() throw(UIException) {
    MessageBox::resize();
    delete cancelbutton;
    cancelbutton = new Button(_("Cancel"), getStartX() + 2 + getOkButtonLength(), getStartY() + getBaseHeight() -2);
}

void
DialogBox::refresh() throw(UIException) {
    MessageBox::refresh();
    cancelbutton->refresh();
}
