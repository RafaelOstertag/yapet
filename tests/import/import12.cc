// $Id$
// Checks whether or not the import of import10 worked.
// Relies on test7.csv

#include <iostream>
#include <typeinfo>

#include "aes256factory.hh"
#include "file.h"

// Keep them define's here, since tests.h only defines the default if they are
// not predefined
#define ROUNDS 200
#define NAME ",Test name %d"
#define HOST "Test, host %d"
#define UNAME "Test username, %d"
#define PW "Test password %d"
#define COMMENT "Test comment %d,"

#include "testpaths.h"
#include "tests.h"

int main(int, char**) {
    try {
        auto password{yapet::toSecureArray("test7")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test7.pet", false};

        std::list<yapet::PasswordListItem> list = file.read();

        if (list.size() != ROUNDS) {
            return 1;
        }

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record(*it, cryptoFactory, i);
            it++;
        }
    } catch (std::exception& ex) {
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
