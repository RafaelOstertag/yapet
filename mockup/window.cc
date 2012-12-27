#include "window.h"

//
// Private
//

//
// Public
//

Window::Window(int nl, int nc, int y, int x) throw(NewWindowFailed) : instances(1) {
    w = newwin(nl, nc, y, x);
    if (w == NULL)
	throw NewWindowFailed();

}

Window::Window(const Window& W) throw() {
    instances = W.instances + 1;
    w = W.w;
}

Window&
Window::operator=(const Window& W) throw() {
    if ( this == &W)
	return *this;

    instances = W.instances + 1;
    w = W.w;
}

Window::~Window() throw(DelWindowFailed) {
    if (instances > 1)
	return;

    int retval = delwin(w);
    if (retval == ERR)
	throw DelWindowFailed();
}

    
