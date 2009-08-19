// $Id$
// Checks whether or not the import of import1 worked.
// Relies on test1.csv

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

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main (int, char**) {
    std::cout << std::endl;
    std::cout << " ==> Check if import1 worked... " << std::endl;

    try {
        YAPET::Key key ("test1");
        YAPET::File file ("test1.pet", key, false);
        std::list<YAPET::PartDec> list = file.read (key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " ==> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, key, i);
            it++;
        }
    } catch (std::exception& ex) {
        std::cout << std::endl;
        std::cout << " ==> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << " ==> yes" << std::endl;
    return 0;
}
