// $Id$
// Checks whether or not the import of import9 worked.
// Relies on test6.csv

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <typeinfo>
#include <cstring>
#include <iostream>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "key.h"
#include "crypt.h"
#include "structs.h"
#include "record.h"
#include "partdec.h"
#include "file.h"

// Keep them define's here, since tests.h only defines the default if they are
// not predefined
#define ROUNDS 200
#define NAME "\"Test name %d"
#define HOST "Test host %d\""
#define UNAME "Test \"username %d"
#define PW "Test \"password\" %d"
#define COMMENT "Test \"\"comment %d\"\""

#include "tests.h"
#include "testpaths.h"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Check if import9 worked... " << std::endl;

    try {
        YAPET::Key key ("test6");
        YAPET::File file ("test6.pet", key, false);
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
        std::cout << typeid (ex).name() << ": "  << ex.what() << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << " ==> yes" << std::endl;
    return 0;

}