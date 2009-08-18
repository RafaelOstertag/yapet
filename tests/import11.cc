// $Id$
//
// Tests whether or not import works with double quoted values and embedded
// delimiters
//
// Relies on test7.csv

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
        std::cout << " ==> Importing from test7.csv" << std::endl;;
        CSVImport imp (SRCDIR "/test7.csv", "test7.pet", ',');
        imp.import ("test7");

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
