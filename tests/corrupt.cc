// $Id$
//
// Checks whether opening corrupt.pet causes an exception to be thrown.
// Corrupt pet has an wrong id string.
//
// Relies on corrupt.pet.

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
#include <file.h>
#include "testpaths.h"

#define FN "corrupt.pet"

int main(int argc, char** argv) {
    try {
	YAPET::Key key("corrupt");
	YAPET::File file(std::string(SRCDIR "/" FN), key, false, false);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 0;
    }
    return 1;
}
