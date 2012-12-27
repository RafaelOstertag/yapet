// -*- mode: c++ -*-

#ifndef WINDOWS_H
#define WINDOWS_H

#include "curex.h"

#include <curses.h>
#ifdef refresh
#undef refresh
#endif

class Window {
    private:
	/// Keeps track of how many instances objects have been created sharing
	/// the same WINDOW structure
	unsigned long instances;
	WINDOW *w;

    public:
	Window(int nl, int nc, int y, int x) throw(NewWindowFailed);
	Window(const Window& W) throw();
	Window& operator=(const Window& W) throw();
	virtual ~Window() throw(DelWindowFailed);

	virtual void show() = 0;
	virtual void refresh() = 0;
virtual 
};

#endif
