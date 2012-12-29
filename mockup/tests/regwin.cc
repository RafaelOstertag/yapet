// $Id$
//
// Test Curses::(un)regwin()

#include <iostream>
#include <exception>

#include "curs.h"

int main() {
    Curses::init();

    try {
	// Expected to throw exception
	Curses::regwin(NULL);
    } catch (std::invalid_argument& e) {
	goto OK1;
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
    }
    goto _ERR;
 OK1:
    try {
	Window w;

	Curses::regwin(&w);
	Curses::unregwin(&w);
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	goto _ERR;
    }

    Curses::end();
    return 0;

 _ERR:
    Curses::end();
    return 1;
}
	
