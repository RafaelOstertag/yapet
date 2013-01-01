// -*- mode: c++ -*-
//
// $Id$

#ifndef WINDOWS_H
#define WINDOWS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "screenobject.h"
#include "margin.h"
#include "curex.h"

#include "mycurses.h"


class Window: public ScreenObject {
    private:
	Margin<> margin;
	bool hasframe;

	void __init();

    public:
	Window();
	Window(const Margin<>& m);
	Window(const Window& W);
	Window& operator=(const Window& W);
	inline bool operator==(const Window& W) const {
	    return getWindow() == W.getWindow();
	}

	inline bool getFrame() const { return hasframe; }
	inline void setFrame(bool b) { hasframe = b; }

	void realize(const Rectangle<>& r);
};

#endif // WINDOWS_H
