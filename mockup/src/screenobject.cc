// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

    Curses::regwin(this);
}

ScreenObject::ScreenObject(const ScreenObject& so) {
    instances = W.instances;
    (*instances)++;
    w = W.w;
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

    Curses::unregwin(this);
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
