// $Id$
//
// Test Curses::(un)regObject()

#include <iostream>
#include <stdexcept>

#include "scrobjlist.h"
#include "curs.h"
#include "window.h"

int main() {

    try {
	// Expected to throw exception
	ScreenObjectList::regObject(NULL);
    } catch (std::invalid_argument&) {
    } catch (BaseCurEx& e) {
    	std::cerr << e.what() << std::endl;
    	return 1;
    }

    try {
	Curses::init(false);
	Window w;

	ScreenObjectList::regObject(&w);
	ScreenObjectList::unregObject(&w);
	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	Curses::end();
	return 1;
    }

    return 0;
}
	
