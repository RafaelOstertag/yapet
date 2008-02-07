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

#include "passwordwidget.h"

using namespace YAPETUI;

PasswordWidget::PasswordWidget(int sx, int sy, int w, int ml) 
    throw(UIException) : InputWidget(sx, sy, w, ml) {}

PasswordWidget::~PasswordWidget() {

}

void
PasswordWidget::refresh() throw(UIException) {
    int retval = wclear(getWindow());
    if (retval == ERR)
	throw UIException("Error clearing input widget");

    if (getPos() > 0) {
	char* tmp = (char*)malloc(getPos()+1);
	memset(tmp, '*', getPos());
	tmp[getPos()] = '\0';
	retval = mymvwaddnstr(getWindow(),
			      0,
			      0,
			      tmp,
			      getWidth()-1);
	free(tmp);
	if (retval == ERR)
	    throw UIException("Error adding text to window");

	if (getPos() >= getWidth() - 1)
	    retval = wmove(getWindow(), 0, getWidth()-1);
	else
	    retval = wmove(getWindow(), 0, getPos());

	if (retval == ERR)
	    throw UIException("Error moving cursor");
    }

    retval = wrefresh(getWindow());
    if (retval == ERR)
	throw UIException("Error refreshing input widget");
}
