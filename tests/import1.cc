// $Id$
// Tests whether or not import works under lab conditions

#include <csvimport.h>

int main (int, char**) {
    try {
	std::cout << "Importing from test1.csv" << std::endl;;
	CSVImport imp("test1.csv", "test1.pet", ',');
	imp.import("test1");
    } catch(std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    return 0;
}
