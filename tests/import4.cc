// $Id$
// Tests whether or not strange input make the import choke
// Relies on test3.csv

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif


#include <csvimport.h>
#include "testpaths.h"

int main (int, char**) {
    try {
	std::cout << "Importing from test3.csv" << std::endl;
	std::cout << "Testing import of strange things" << std::endl;
	CSVImport imp( SRCDIR "/test3.csv", "test3.pet", ',');
	imp.import("test3");
	if (imp.hadErrors()) {
	    imp.printLog();
	    return 1;
	}
    } catch(std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    return 0;
}
