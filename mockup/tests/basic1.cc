// $Id$
//
// Test basic functionality of Curses class

#include <unistd.h>

#include <iostream>

#include "curs.h"
#include "window.h"

int main() {
    try {
	Curses::init(false);
	Curses::setTitle("Basic 1");

	Window w1;
	w1.setFrame(true);

	Curses::show();

	sleep(2);

	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	sleep(2);
	goto _ERR;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}
