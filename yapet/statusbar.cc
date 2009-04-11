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
#include "statusbar.h"

void
StatusBar::createWindow() throw(YAPETUI::UIException){
    if (statusbar != NULL)
	throw YAPETUI::UIException(_("May you consider deleting the window before allocating"));

    statusbar = newwin (1, maxX(), maxY() - 1, minX());
    if (statusbar == NULL)
	throw YAPETUI::UIException (_("statusbar could not be initialized"));

    int retval = wattron (statusbar, A_REVERSE);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error setting attribute"));

    retval = wbkgd (statusbar, ' ' | A_REVERSE);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error setting the statusbar background"));

    refresh();
}

StatusBar::StatusBar() throw (YAPETUI::UIException) : BaseWindow(),
						      statusbar(NULL) {
    createWindow();
}

StatusBar::~StatusBar() {
    wclear(statusbar);
	delwin (statusbar);
}

void
StatusBar::putMsg (std::string msg) throw (YAPETUI::UIException) {
    message = msg;
    int retval = wclear (statusbar);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error erasing status bar"));
    retval = mywaddstr (statusbar, message.c_str());
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error adding status message"));
    retval = wrefresh(statusbar);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error refreshing status bar"));
}

void
StatusBar::refresh() {
    // Does a refresh
    putMsg (message);
}

void
StatusBar::resize() {
    int retval = delwin(statusbar);
    if (retval == ERR)
	throw YAPETUI::UIException(_("status bar could not be deleted"));

    statusbar = NULL;

    createWindow();
}
