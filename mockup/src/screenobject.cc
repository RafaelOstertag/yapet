// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "scrobjlist.h"
#include "curs.h"
#include "screenobject.h"

//
// Private
//

//
// Protected
//

//
// Public
//

ScreenObject::ScreenObject() : instances(NULL),
			       w(NULL) {
    w = new WINDOW*;
    *w = NULL; // resize has to take care of allocating the window
    
    instances = new unsigned int;
    *instances = 1;

    ScreenObjectList::regObject(this);
}

ScreenObject::ScreenObject(const ScreenObject& so) {
    instances = so.instances;
    (*instances)++;
    w = so.w;
}

ScreenObject::~ScreenObject() {
    if (*instances > 1) {
	(*instances)--;
	return;
    }

    delete instances;

    if (*w != NULL) {
	int retval = delwin(*w);
	if (retval == ERR)
	    throw DelWindowFailed();
    }

    delete w;

    ScreenObjectList::unregObject(this);
}    

ScreenObject&
ScreenObject::operator=(const ScreenObject& so) {
    if ( this == &so)
	return *this;

    instances = so.instances;
    (*instances)++;

    w = so.w;

    return *this;
}

void
ScreenObject::refresh() {
    int retval = wnoutrefresh(getWindow());
    if (retval == ERR)
	throw RefreshFailed();
}

void
ScreenObject::resize(const Dimension& d) {

    if (*w != NULL) {
	int retval = delwin(*w);
	if (retval == ERR)
	    throw DelWindowFailed();
    }

    *w = newwin(d.getLines(),
		d.getCols(),
		d.getY(),
		d.getX());
    if (*w == NULL)
	throw NewWindowFailed();
}
