// $Id$
// Checks whether or not the import of import5 worked.
// Relies on test4.csv

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <typeinfo>
#include <iostream>
#include <cstring>

#include <unistd.h>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "key.h"
#include "crypt.h"
#include "structs.h"
#include "record.h"
#include "partdec.h"
#include "file.h"

#include "tests.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Check if import5 worked" << std::endl;

    try {
        YAPET::Key key ("test4");
        YAPET::File file ("test4.pet", key, false);
        std::list<YAPET::PartDec> list = file.read (key);

        if (list.size() != ROUNDS)
            return 1;

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
