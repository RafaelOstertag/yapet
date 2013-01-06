// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CERRNO
#include <cerrno>
#endif // HAVE_CERRNO

#include "curex.h"
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

ScreenObject::ScreenObject(): Realizeable(),
    rect(),
    instances(NULL),
    w(NULL)
{
    w = new WINDOW*;
    *w = NULL; // resize has to take care of allocating the window

    instances = new unsigned int;
    *instances = 1;
}

ScreenObject::ScreenObject(const ScreenObject& so) : Realizeable(so) {
    instances = so.instances;
    (*instances)++;

    w = so.w;

    rect = so.rect;
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
}

ScreenObject&
ScreenObject::operator=(const ScreenObject& so) {
    if ( this == &so)
	return *this;

    Realizeable::operator=(so);

    instances = so.instances;
    (*instances)++;

    w = so.w;

    rect = so.rect;

    return *this;
}

void
ScreenObject::refresh() {
    int retval = wnoutrefresh(*w);
    if (retval == ERR)
	throw RefreshFailed();
}

void
ScreenObject::resize(const Rectangle<>& r) {
    if (!isRealized()) throw NotRealized();

    setRealized(false);

    rect = r;

    int retval = delwin(*w);
    if (retval == ERR) {
	throw DelWindowFailed();
    }

    realize(r);

    refresh();
}

void
ScreenObject::realize(const Rectangle<>& r) {
    if (isRealized()) return;

    rect = r;

    *w = newwin(rect.getLines(),
		rect.getCols(),
		rect.getY(),
		rect.getX());
    if (*w == NULL) {
	throw NewWindowFailed();
    }

    setRealized(true);
}
