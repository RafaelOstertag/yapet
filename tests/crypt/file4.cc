// $Id$
/*
 * Depends on the file created by test file3
 */

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

#include "tests.h"
#include "testpaths.h"

// Please note: The file path of the test file cannot point to SRCDIR as
// specified in testpaths.h since this breaks 'make distcheck'

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Be patient, this test may take a few moments ..." << std::endl;

    try {
        YAPET::Key key ("JustAPassword");
        YAPET::File file (FN, key, false);
	assert(file.getFileVersion(key) == YAPET::VERSION_2);
        std::list<YAPET::PartDec> list = file.read (key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << "List has unexpected size" << std::endl;
            return 1;
        }

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
