// $Id$
// Tests the error detection.
// Relies on test2.csv

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
        std::cout << " ==> Importing from test2.csv" << std::endl;
        std::cout << " ==> Testing for error recognition" << std::endl;
        CSVImport imp (SRCDIR "/test2.csv", "test2.pet", ',');
        imp.import ("test2");
        imp.printLog();

        if (imp.numErrors() != 2)
            return 1;

        std::list<CSVImport::LogEntry> logs (imp.getLog() );

        if (logs.size() != 2)
            return 1;

        std::list<CSVImport::LogEntry>::const_iterator it = logs.begin();

        if ( (*it).lineno != 2)
            return 1;

        it++;

        if ( (*it).lineno != 10)
            return 1;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
