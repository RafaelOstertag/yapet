// $Id$
//
// Test initialization
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#if HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "curs.h"

int main() {
    try {
	Curses::init();
	sleep(1);
	Curses::end();
    } catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
