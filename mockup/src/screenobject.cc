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

void
ScreenObject::blocksignal() {
#ifdef SIGWINCH
    if (signal_blocked) return;
    int err = sigprocmask(SIG_BLOCK, &block_sigset, &old_sigset);
    if (err)
	throw SystemError(errno);
    signal_blocked = true;
#endif // SIGWINCH
}

void
ScreenObject::unblocksignal() {
#ifdef SIGWINCH
    if (!signal_blocked) return;
    signal_blocked = false;

    int err = sigprocmask(SIG_SETMASK, &old_sigset, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH
}


//
// Public
//

ScreenObject::ScreenObject(): Realizeable(),
    rect(),
    instances(NULL),
    w(NULL),
    signal_blocked(false)
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
