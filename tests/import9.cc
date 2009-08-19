// $Id$
//
// Tests whether or not import works with double quoted values and escaped
// double quotes.
//
// Relies on test6.csv

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
	std::cout << " ==> Importing from test6.csv" << std::endl;;
	CSVImport imp (SRCDIR "/test6.csv", "test6.pet", ',');
	imp.import ("test6");

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
