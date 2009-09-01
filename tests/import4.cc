// $Id$
// Tests whether or not strange input make the import choke
// Relies on test3.csv

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <typeinfo>

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <csvimport.h>
#include "testpaths.h"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        std::cout << " ==> Importing from test3.csv" << std::endl;
        std::cout << " ==> Testing import of strange things" << std::endl;
        CSVImport imp ( SRCDIR "/test3.csv", "test3.pet", ',');
        imp.import ("test3");

        if (imp.hadErrors() ) {
            imp.printLog();
            return 1;
        }
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
