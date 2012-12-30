// -*- mode: c++ -*-
//
// $Id$

#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mycurses.h"
#include "dimension.h"

class ScreenObject {
    private:
	/// Keeps track of how many instances objects have been created sharing
	/// the same WINDOW structure
	unsigned int* instances;
	/// Pointer to a pointer so that we can replace the window for all
	/// instances simultaneously.
	WINDOW** w;

    protected:
	inline WINDOW* getWindow() const { return *w; }

	/// Keep this. Used by tests/windowrefs.cc.
	inline unsigned int getInstanceCount() const {
	    return *instances;
	}

	void resize(const Dimension& d);
    public:
	ScreenObject();
	ScreenObject(const ScreenObject& so);
	virtual ~ScreenObject();
	ScreenObject& operator=(const ScreenObject& so);


	virtual void refresh();
	virtual void resize() = 0;
};
#endif
