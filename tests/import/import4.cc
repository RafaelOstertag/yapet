// $Id$
// Tests whether or not strange input make the import choke
// Relies on test3.csv

#include <iostream>

#include "csvimport.h"
#include "testpaths.h"

int main(int, char**) {
    try {
        CSVImport imp(SRCDIR "/test3.csv", "test3.pet", ',');
        imp.import("test3");

        if (imp.hadErrors()) {
            imp.printLog();
            return 1;
        }
    } catch (std::exception& ex) {
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
