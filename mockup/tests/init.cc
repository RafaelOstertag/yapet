// $Id$
//
// Test initialization
#include <unistd.h>

#include <iostream>
#include <exception>
#include "curs.h"

int main() {
    try {
	Curses::init(false);
	sleep(1);
	Curses::end();
    } catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
