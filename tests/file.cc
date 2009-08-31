// $Id$

#include <typeinfo>

#include <iostream>
#include <file.h>
#include <unistd.h>
#include "testpaths.h"

#define FN "testfile.gps"

// Please note: The file path of the test file cannot point to SRCDIR as
// specified in testpaths.h since this breaks 'make distcheck'

int main (int, char**) {
#ifndef TESTS_VERBOSE
    close(STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        YAPET::Key key ("JustATestPasswordForKeepingSecret");
        YAPET::File file (std::string (FN), key, true);
	assert(file.getFileVersion(key) == YAPET::VERSION_2);
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
