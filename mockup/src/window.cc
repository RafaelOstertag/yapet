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

//
// Public
//

Window::Window(const Margin& m) : ScreenObject(),
    margin(m),
    hasframe(false) {

    resize();
}

Window::Window() {
    Window::Window(Margin());
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

Window::~Window() {

}

void
Window::refresh() {
    int retval;

    if (hasframe) {
	retval = box(*w, 0, 0);
	if (retval == ERR)
	    throw BoxFailed();
    }

    retval = wnoutrefresh(*w);
    if (retval == ERR)
	throw RefreshFailed();
}

void
Window::resize() {
    if (*w != NULL) {
	int retval = delwin(*w);
	if (retval == ERR)
	    throw DelWindowFailed();
    }

    Dimension _d = Curses::getDimension() - margin;
    
    *w = newwin(_d.getLines(),
		_d.getCols(),
		_d.getY(),
		_d.getX());
    if (*w == NULL)
	throw NewWindowFailed();
}
