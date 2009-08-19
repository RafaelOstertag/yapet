// $Id$
//
// Testing whether or not files generated on the author's machine can be read
// on other machines as well...
//
// Relies on foreign.pet

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
    std::cout << " ==> Check if we can read files created on a different machine... " << std::endl;

    try {
        YAPET::Key key ("foreign");
        YAPET::File file (SRCDIR"/foreign.pet", key, false, false);
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
