// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "curs.h"
#include "window.h"
#include "rectangle.h"

//
// Private
//

//
// Public
//

Window::Window(const Margin<>& m) : ScreenObject(),
				    margin(m),
				    hasframe(false) {
}

Window::Window() : ScreenObject(),
		   margin(Margin<>()),
		   hasframe(false) {
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
Window::realize(const Rectangle<>& r) {
    ScreenObject::realize(r);

    if (hasframe) {
	blocksignal();
	int retval = box(getWindow(), 0, 0);
	unblocksignal();
	if (retval == ERR)
	    throw BoxFailed();
    }
}
