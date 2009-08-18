// $Id$
//
// Tests whether or not a wrong password throws an exception.

#include <typeinfo>

#include <iostream>
#include <file.h>
#include "testpaths.h"

#define FN "testfile.gps"

// Please note: The file path of the test file cannot point to SRCDIR as
// specified in testpaths.h since this breaks 'make distcheck'

int main (int argc, char** argv) {
    try {
        YAPET::Key key ("JustATestPasswordForKeepingSecret");
        YAPET::File file (std::string (FN), key, false);
    } catch (std::exception& ex) {
        std::cout << " ##> " << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    try {
        YAPET::Key key2 ("JustATestPasswordForKeepingSecreT");
        YAPET::File file2 (std::string (FN), key2, false);
    } catch (std::exception& ex) {
        std::cout << " ==> " << typeid (ex).name() << ": " << ex.what() << " (OK) " << std::endl;
        return 0;
    }

    return 1;
}
