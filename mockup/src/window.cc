// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "curs.h"
#include "window.h"
#include "dimension.h"

//
// Private
//
void
Window::__init() {
    resize();

    int retval = scrollok(getWindow(), FALSE);
    if (retval == ERR)
	throw ScrollOKFailed();

    retval = leaveok(getWindow(), FALSE);
    if (retval == ERR)
	throw LeaveOKFailed();
}

//
// Public
//

Window::Window(const Margin& m) : ScreenObject(),
				  margin(m),
				  hasframe(false) {
    __init();
}

Window::Window() : ScreenObject(),
		   margin(Margin()),
		   hasframe(false) {
    __init();
}

Window::Window(const Window& W) : ScreenObject(W) {
    hasframe = W.hasframe;
    margin = W.margin;
}

Window&
Window::operator=(const Window& W) {
    ScreenObject::operator=(W);

    hasframe = W.hasframe;
    margin = W.margin;

    return *this;
}

void
Window::refresh() {
    int retval;

    if (hasframe) {
	retval = box(getWindow(), 0, 0);
	if (retval == ERR)
	    throw BoxFailed();
    }

    ScreenObject::refresh();
}

void
Window::resize() {
    ScreenObject::resize(Curses::getDimension() - margin);
}
