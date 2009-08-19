// $Id$
//
// Checks whether opening corrupt.pet causes an exception to be thrown.
// Corrupt pet has an wrong id string.
//
// Relies on corrupt.pet.

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <typeinfo>
#include <iostream>
#include <file.h>
#include "testpaths.h"

#define FN "corrupt.pet"

int main(int, char**) {
    std::cout << std::endl;
    try {
	YAPET::Key key ("corrupt");
	YAPET::File file (std::string (SRCDIR "/" FN), key, false, false);
    } catch (std::exception& ex) {
	// Everything fine...
	std::cout << " ==> " << typeid (ex).name() << ": " << ex.what() << " (OK) " << std::endl;
	return 0;
    }

    return 1;
}
