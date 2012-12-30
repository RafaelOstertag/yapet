// -*- mode: c++ -*-
//
// $Id$

#ifndef SCROBJLIST_H
#define SCROBJLIST_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>

#include "screenobject.h"

class ScreenObjectList {
    private:
	static std::list<ScreenObject*> object_list;

    public:
	static void regObject(ScreenObject* o);
	static void unregObject(ScreenObject* o);
        static void refreshAll();
};

#endif
