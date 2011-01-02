// $Id$
//
// Checks whether opening corrupt.pet causes an exception to be thrown.
// Corrupt pet has an wrong id string.
//
// Relies on corrupt.pet.

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <typeinfo>
#include <iostream>



#include <file.h>

#define FN "corrupt.pet"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd ,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        YAPET::Key key ("corrupt");
        YAPET::File file (std::string (FN), key, false, false);
    } catch (std::exception& ex) {
        // Everything fine...
        std::cout << " ==> " << typeid (ex).name() << ": " << ex.what() << " (OK) " << std::endl;
        return 0;
    }

    return 1;

}
