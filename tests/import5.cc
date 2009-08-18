// $Id$
// Tests whether or not import works with different separator
// Relies on test4.csv

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
    try {
        std::cout << " ==> Importing from test4.csv" << std::endl;
        std::cout << " ==> Testing different (;) separator" << std::endl;
        CSVImport imp (SRCDIR "/test4.csv", "test4.pet", ';');
        imp.import ("test4");

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
