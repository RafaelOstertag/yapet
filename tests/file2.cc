// $Id$

#include <iostream>
#include <file.h>
#include "testpaths.h"

#define FN "testfile.gps"

int main(int argc, char** argv) {
    try {
	YAPET::Key key("JustATestPasswordForKeepingSecret");
	YAPET::File file(std::string(SRCDIR "/" FN), key, false);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    try {
	YAPET::Key key2("JustATestPasswordForKeepingSecreT");
	YAPET::File file2(std::string(SRCDIR "/" FN), key2, false);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 0;
    }

    return 1;
}
