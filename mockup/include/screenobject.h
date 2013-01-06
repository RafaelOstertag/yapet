// -*- mode: c++ -*-
//
// $Id$

#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mycurses.h"
#include "realizeable.h"

class ScreenObject : public Realizeable {
    private:
	Rectangle<> rect;
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

    public:
	ScreenObject();
	ScreenObject(const ScreenObject& so);
	virtual ~ScreenObject();
	ScreenObject& operator=(const ScreenObject& so);

	void refresh();
	void resize(const Rectangle<>& r);
	void realize(const Rectangle<>& r);
};

#endif // SCREENOBJECT_H
