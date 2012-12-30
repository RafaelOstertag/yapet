// $Id$
//
// Test reference count of window class
#include <cstdlib>

#include <iostream>
#include <exception>

#include "curs.h"
#include "window.h"


class TWindow: public Window {
    public:
	inline unsigned int _test() const {
	    return getInstanceCount();
	}
};

void f1(TWindow w) {
    if (w._test() != 3)
	std::exit(1);
}

int main() {
    try {
	Curses::init(false);

	TWindow t0;
	if (t0._test() != 1)
	    goto _ERR;

	TWindow t1 = t0;
	if (t1._test() != 2 || t0._test() != 2)
	    goto _ERR;

	f1(t1);

	if (t1._test() != 2 || t0._test() != 2)
	    goto _ERR;

	TWindow* t2 = new TWindow(t1);
	if (t0._test() != 3 ||
	    t1._test() != 3 ||
	    t2->_test() != 3)
	    goto _ERR;

	delete t2;
	if (t1._test() != 2 || t0._test() != 2)
	    goto _ERR;
	
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
    
