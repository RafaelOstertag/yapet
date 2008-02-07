// $Id$

#include <iostream>
#include <file.h>

#define FN "testfile.gps"

int main(int argc, char** argv) {
    try {
	YAPET::Key key("JustATestPasswordForKeepingSecret");
	YAPET::File file(std::string(FN), key, true);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
