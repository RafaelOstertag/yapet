// $Id$
// Checks whether or not the import of import5 worked.
// Relies on test4.csv

#include <iostream>
#include <typeinfo>

#include "aes256factory.hh"
#include "file.h"

#include "tests.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main(int, char**) {
    try {
        auto password{yapet::toSecureArray("test4")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test4.pet", false};
        std::list<yapet::PasswordListItem> list = file.read();

        if (list.size() != ROUNDS) return 1;

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
