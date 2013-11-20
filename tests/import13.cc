// $Id$
//
// Tests the error detection when double quoting is wrong.
//
// Relies on test8.csv

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <typeinfo>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <unistd.h>

#include "csvimport.h"
#include "testpaths.h"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

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
