// $Id$
// Tests whether or not import works under lab conditions.
// Relies on test1.csv

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif

#include <typeinfo>

#include <csvimport.h>

#include "testpaths.h"

int main (int, char**) {
    std::cout << std::endl;
    try {
	std::cout << " ==> Importing from test1.csv" << std::endl;;
	CSVImport imp (SRCDIR "/test1.csv", "test1.pet", ',');
	imp.import ("test1");

	if (imp.hadErrors() )
	    return 1;

	if (imp.numErrors() != 0)
	    return 1;
    } catch (std::exception& ex) {
	std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	return 1;
    }

    return 0;
}
