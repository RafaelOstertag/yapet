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

#include "intl.h"
#include "searchdialog.h"

void
SearchDialog::createWindow() throw(YAPETUI::UIException) {
    if (window != NULL)
	throw YAPETUI::UIException(_("May you consider deleting the window before reallocating"));

    window = newwin(getHeight(), getWidth(), getStartY(), getStartX());
    if (window == NULL)
	throw YAPETUI::UIException(_("Error creating search dialog"));

    searchtermw = new YAPETUI::InputWidget(getStartX() + 1,
					 getStartY() + 2,
					 getWidth() - 2);

    okbutton = new YAPETUI::Button(_("OK"),
				   getStartX() + 1,
				   getStartY() + getHeight() - 2);

    cancelbutton = new YAPETUI::Button(_("Cancel"),
				       getStartX() + okbutton->getLength() + 2,
				       getStartY() + getHeight() - 2);
}

SearchDialog::SearchDialog() throw(YAPETUI::UIException) : window(NULL),
							   searchtermw(NULL),
							   okbutton(NULL),
							   cancelbutton(NULL),
							   searchterm(""),
							   canceled(true) {
    createWindow();
}

SearchDialog::~SearchDialog() {
    delete searchtermw;
    delete okbutton;
    delete cancelbutton;
    delwin(window);
}

void
SearchDialog::run() throw(YAPETUI::UIException) {
    refresh();
    while (true) {
	int ch = 0;
#ifdef HAVE_WRESIZE
	while ( (ch = searchtermw->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	searchtermw->focus();
#endif // HAVE_WRESIZE

#ifdef HAVE_WRESIZE
	while ( (ch = okbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = okbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n') {
	    canceled = false;
	    return;
	}
#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE)
	    YAPETUI::BaseWindow::resizeAll();
#else // HAVE_WRESIZE
	ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n') {
	    canceled = true;
	    return;
	}
    }

}

void
SearchDialog::resize() throw(YAPETUI::UIException) {
    int retval = delwin(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error deleting search dialog window"));

    delete searchtermw;
    delete okbutton;
    delete cancelbutton;

    window = NULL;
    searchtermw = NULL;
    okbutton = NULL;
    cancelbutton = NULL;

    createWindow();
}

void
SearchDialog::refresh() throw(YAPETUI::UIException) {
    YAPETUI::Colors::setcolor(window, YAPETUI::MESSAGEBOX);
    int retval = werase(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error clearing search dialog"));

    retval = box(window, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error adding box"));

    retval = mymvwaddstr(window, 0, 2, _("S E A R C H"));
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error setting title"));

    // The label
#ifdef HAVE_STRCASESTR
    retval = mymvwaddstr(window, 1, 1, _("Please enter the search term"));
#else
    retval = mymvwaddstr(window, 1, 1, _("Please enter the search term (case-sensitive)"));
#endif
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error setting label"));

    retval = wrefresh(window);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error refreshing the search dialog"));

    searchtermw->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}
