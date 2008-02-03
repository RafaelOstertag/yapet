// $Id$

#include <iostream>
#include <file.h>

#define FN "testfile.gps"

int main(int argc, char** argv) {
    try {
	GPSAFE::Key key("JustATestPasswordForKeepingSecret");
	GPSAFE::File file(std::string(FN), key, false);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    try {
	GPSAFE::Key key2("JustATestPasswordForKeepingSecreT");
	GPSAFE::File file2(std::string(FN), key2, false);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 0;
    }

    return 1;
}
