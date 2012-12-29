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
	Margin margin;

	bool hasframe;

    public:
	Window(const Margin& m);
	Window();
	Window(const Window& W);
	Window& operator=(const Window& W);
	inline bool operator==(const Window& W) const {
	    return w == W.w;
	}
	virtual ~Window();

	inline bool getFrame() const { return hasframe; }
	inline void setFrame(bool b) { hasframe = b; }
	virtual void refresh();
	virtual void resize();
};

#endif
