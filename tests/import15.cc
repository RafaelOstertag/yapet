// $Id: import14.cc 5623 2013-11-21 14:36:29Z rafisol $
//
// Test 

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <typeinfo>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "csvimport.h"
#include "testpaths.h"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        std::cout << " ==> Importing from test10.csv" << std::endl;
        std::cout << " ==> Testing import of strange things" << std::endl;
	CSVImport imp ( SRCDIR "/test10.csv", "test10.pet", ';');
        imp.import ("test10");

        if (imp.hadErrors() ) {
            imp.printLog();
            return 1;
        }

        if (imp.numErrors() != 0)
            return 1;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;

}
