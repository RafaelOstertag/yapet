// $Id$
// Checks whether or not the import of import1 worked

#include <string.h>

#include <iostream>

#include <key.h>
#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <partdec.h>
#include <file.h>

#include "tests.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main(int, char**) {
    std::cout << "Check if import1 worked" << std::endl;
    try {
	YAPET::Key key("test1");
	YAPET::File file("test1.pet", key, false);
	std::list<YAPET::PartDec> list = file.read(key);
	if (list.size() != ROUNDS)
	    return 1;

	std::list<YAPET::PartDec>::iterator it = list.begin();

	for(int i=0; it != list.end(); i++) {
	    check_record(*it, key, i);
	    it++;
	}

    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
