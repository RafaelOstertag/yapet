// $Id$
// Checks whether or not the import of import5 worked.
// Relies on test4.csv

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstring>
#include <iostream>
#include <typeinfo>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "blowfishfactory.hh"
#include "file.h"

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
    std::cout << " ==> Check if import5 worked" << std::endl;

    try {
        auto password{yapet::toSecureArray("test4")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::BlowfishFactory{password}};

        YAPET::File file{cryptoFactory, "test4.pet", false};
        std::list<yapet::PasswordListItem> list = file.read();

        if (list.size() != ROUNDS) return 1;

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record(*it, cryptoFactory, i);
            it++;
        }
    } catch (std::exception& ex) {
        std::cout << std::endl;
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    std::cout << std::endl;
    return 0;
}
