// $Id$
// Checks whether or not the import of import7 worked.
// Relies on test5.csv

#include <string.h>

#include <iostream>

#include <key.h>
#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <partdec.h>
#include <file.h>

#include "tests.h"
#include "testpaths.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main(int, char**) {
    std::cout << " ==> Check if import7 worked... " << std::endl;
    try {
	YAPET::Key key("test5");
	YAPET::File file("test5.pet", key, false);
	std::list<YAPET::PartDec> list = file.read(key);
	if (list.size() != ROUNDS) {
	    std::cout << "no" << std::endl;
	    return 1;
	}

	std::list<YAPET::PartDec>::iterator it = list.begin();

	for(int i=0; it != list.end(); i++) {
	    check_record(*it, key, i);
	    it++;
	}

    } catch (std::exception& ex) {
	std::cout << "no" << std::endl;
	std::cout << ex.what() << std::endl;
	return 1;
    }
    std::cout << "yes" << std::endl;
    return 0;
}
