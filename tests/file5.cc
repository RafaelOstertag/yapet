// $Id$
/*
 * Depends on the file created by test file3
 */

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

int main(int, char**) {
    std::cout << " ==> Be patient, this test may take a few moments ..." << std::endl;
    try {
	YAPET::Key oldkey("JustAPassword");
	YAPET::Key newkey("JustANewPassword");
	YAPET::File file(FN, oldkey, false);
	file.setNewKey(oldkey, newkey);
	std::list<YAPET::PartDec> list = file.read(newkey);
	if (list.size() != ROUNDS)
	    return 1;

	std::list<YAPET::PartDec>::iterator it = list.begin();
	for(int i=0; it != list.end(); i++) {
	    check_record(*it, newkey, i);
	    it++;
	}

    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
