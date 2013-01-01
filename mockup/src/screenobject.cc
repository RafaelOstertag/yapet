// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>

#include "curex.h"
#include "screenobject.h"

//
// Private
//

//
// Protected
//
#ifdef SIGWINCH
void
ScreenObject::blocksignal() {
    int err = sigprocmask(SIG_BLOCK, &block_sigset, NULL);
    if (err)
	throw SystemError(errno);
}

void
ScreenObject::unblocksignal() {
    int err = sigprocmask(SIG_UNBLOCK, &block_sigset, NULL);
    if (err)
	throw SystemError(errno);
}

#endif // SIGWINCH
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

#ifdef SIGWINCH
    sigemptyset(&block_sigset);
    sigaddset(&block_sigset, SIGWINCH);
#endif // SIGWINCH
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
	blocksignal();
	int retval = delwin(*w);
	unblocksignal();
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
    blocksignal();
    int retval = wnoutrefresh(*w);
    unblocksignal();
    if (retval == ERR)
	throw RefreshFailed();
}

void
ScreenObject::resize(const Rectangle<>& r) {
    if (!isRealized()) throw NotRealized();
    blocksignal();

    setRealized(false);

    rect = r;

    int retval = delwin(*w);
    if (retval == ERR) {
	unblocksignal();
	throw DelWindowFailed();
    }

    realize(r);

    refresh();
    unblocksignal();
}

void
ScreenObject::realize(const Rectangle<>& r) {
    if (isRealized()) return;
    blocksignal();

    rect = r;

    *w = newwin(rect.getLines(),
		rect.getCols(),
		rect.getY(),
		rect.getX());
    if (*w == NULL) {
	unblocksignal();
	throw NewWindowFailed();
    }

    setRealized(true);
    unblocksignal();
}
