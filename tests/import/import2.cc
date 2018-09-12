// $Id$
// Checks whether or not the import of import1 worked.
// Relies on test1.csv

#include "aes256factory.hh"
#include "file.h"

#include "testpaths.h"
#include "tests.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main(int, char**) {
    try {
        auto password{yapet::toSecureArray("test1")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        auto crypto{cryptoFactory->crypto()};

        std::shared_ptr<YAPET::File> file{
            new YAPET::File{cryptoFactory, "test1.pet", false}};
        std::list<yapet::PasswordListItem> list = file->read();

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
