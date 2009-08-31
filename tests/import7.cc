// $Id$
// Tests whether or not import works with double quoted values
// Relies on test5.csv

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif
#include <unistd.h>
#include <typeinfo>

#include <csvimport.h>

#include "testpaths.h"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    close(STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        std::cout << " ==> Importing from test5.csv" << std::endl;;
        CSVImport imp (SRCDIR "/test5.csv", "test5.pet", ',');
        imp.import ("test5");

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
