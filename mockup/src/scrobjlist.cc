// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include <algorithm>

#include "scrobjlist.h"

std::list<ScreenObject*> ScreenObjectList::object_list;

/// Function object
class RefreshObject {
    public:
	inline void operator()(ScreenObject* o) {
	    o->refresh();
	}
};

//
// Public
//
void
ScreenObjectList::regObject(ScreenObject* o) {
     if (!o)
	throw std::invalid_argument("Argument cannot be null ptr");

    // New ScreenObjects will be pushed at the back, so that they will be displayed
    // last and not obscured by earlier ScreenObjects.
    object_list.push_back(o);
}

void
ScreenObjectList::unregObject(ScreenObject* o) {
     object_list.remove(o);
}

void
ScreenObjectList::refreshAll() {
     std::for_each(object_list.begin(), object_list.end(), RefreshObject());
}

