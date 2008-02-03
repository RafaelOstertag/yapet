// $Id$

#include "passwordwidget.h"

using namespace GPSUI;

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
