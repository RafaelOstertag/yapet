// $Id$
/*
 * Depends on the file created by test file3
 */

#include <typeinfo>

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

// Please note: The file path of the test file cannot point to SRCDIR as
// specified in testpaths.h since this breaks 'make distcheck'

int main (int, char**) {
    std::cout << std::endl;
    std::cout << " ==> Be patient, this test may take a few moments ..." << std::endl;

    try {
	YAPET::Key key ("JustAPassword");
	YAPET::File file (FN, key, false);
	std::list<YAPET::PartDec> list = file.read (key);

	if (list.size() != ROUNDS) {
	    std::cout << std::endl;
	    std::cout << "List has unexpected size" << std::endl;
	    return 1;
	}

	std::list<YAPET::PartDec>::iterator it = list.begin();

	for (int i = 0; it != list.end(); i++) {
	    check_record (*it, key, i);
	    it++;
	}
    } catch (std::exception& ex) {
	std::cout << std::endl;
	std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	return 1;
    }

    std::cout << std::endl;
    return 0;
}
