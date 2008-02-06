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

#include "statusbar.h"

void
StatusBar::createWindow() throw(GPSUI::UIException){
    if (statusbar != NULL)
	throw GPSUI::UIException("May you consider deleting the window before allocating");

    statusbar = newwin (1, maxX(), maxY() - 1, minX());
    if (statusbar == NULL)
        throw GPSUI::UIException ("statusbar could not be initialized");

    int retval = wattron (statusbar, A_REVERSE);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting attribute");

    retval = wbkgd (statusbar, ' ' | A_REVERSE);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting the statusbar background");

    refresh();
}

StatusBar::StatusBar() throw (GPSUI::UIException) : Resizeable(),
						    statusbar(NULL) {
    createWindow();
}

StatusBar::~StatusBar() {
	wclear(statusbar);
	wrefresh(statusbar);
    delwin (statusbar);
}

void
StatusBar::putMsg (std::string msg) throw (GPSUI::UIException) {
    message = msg;
    int retval = wclear (statusbar);
    if (retval == ERR)
        throw GPSUI::UIException ("Error erasing status bar");
    retval = mywaddstr (statusbar, message.c_str());
    if (retval == ERR)
        throw GPSUI::UIException ("Error adding status message");
    retval = wrefresh(statusbar);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing status bar");
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
	throw GPSUI::UIException("status bar could not be deleted");

    statusbar = NULL;

    createWindow();
}
