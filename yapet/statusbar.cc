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

#include "../intl.h"
#include "statusbar.h"

void
StatusBar::createWindow() throw (YAPET::UI::UIException) {
    if (statusbar != NULL)
        throw YAPET::UI::UIException (_ ("May you consider deleting the window before allocating") );

    statusbar = newwin (1, maxX(), maxY() - 1, minX() );

    if (statusbar == NULL)
        throw YAPET::UI::UIException (_ ("statusbar could not be initialized") );

    int retval = wattron (statusbar, A_REVERSE);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error setting attribute") );

    retval = wbkgd (statusbar, ' ' | A_REVERSE);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error setting the statusbar background") );

    refresh();
}

StatusBar::StatusBar() throw (YAPET::UI::UIException) : BaseWindow(),
        statusbar (NULL) {
    createWindow();
}

StatusBar::~StatusBar() {
    wclear (statusbar);
    delwin (statusbar);
}

void
StatusBar::putMsg (std::string msg) throw (YAPET::UI::UIException) {
    message = msg;
    int retval = wclear (statusbar);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error erasing status bar") );

    retval = mywaddstr (statusbar, message.c_str() );

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error adding status message") );

    retval = wrefresh (statusbar);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error refreshing status bar") );
}

void
StatusBar::refresh() {
    // Does a refresh
    putMsg (message);
}

void
StatusBar::resize() {
    int retval = delwin (statusbar);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("status bar could not be deleted") );

    statusbar = NULL;
    createWindow();
}
