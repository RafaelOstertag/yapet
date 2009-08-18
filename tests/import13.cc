// $Id$
//
// Tests the error detection when double quoting is wrong.
//
// Relies on test8.csv

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
        std::cout << " ==> Importing from test8.csv" << std::endl;
        std::cout << " ==> Testing for error recognition" << std::endl;
        CSVImport imp (SRCDIR "/test8.csv", "test8.pet", ',');
        imp.import ("test8");
        imp.printLog();

        if (imp.numErrors() != 1)
            return 1;

        std::list<CSVImport::LogEntry> logs (imp.getLog() );

        if (logs.size() != 1)
            return 1;

        std::list<CSVImport::LogEntry>::const_iterator it = logs.begin();

        if ( (*it).lineno != 3)
            return 1;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
