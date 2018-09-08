// $Id$
// Checks whether or not the import of import1 worked.
// Relies on test1.csv

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <typeinfo>

#include <cstring>
#include <iostream>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "blowfishfactory.hh"
#include "crypt.h"
#include "file.h"

#include "testpaths.h"
#include "tests.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main(int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd, STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Check if import1 worked... " << std::endl;

    try {
        auto password{yapet::toSecureArray("test1")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::BlowfishFactory{password}};

        auto crypto{cryptoFactory->crypto()};

        std::shared_ptr<YAPET::File> file{
            new YAPET::File{cryptoFactory, "test1.pet", false}};
        std::list<yapet::PasswordListItem> list = file->read();

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " ==> no" << std::endl;
            return 1;
        }

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record(*it, cryptoFactory, i);
            it++;
        }
    } catch (std::exception& ex) {
        std::cout << std::endl;
        std::cout << " ==> no" << std::endl;
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << " ==> yes" << std::endl;
    return 0;
}
