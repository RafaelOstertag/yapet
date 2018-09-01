// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstring>
#include <typeinfo>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "file.h"

#include "testpaths.h"
#include "tests.h"

// Please note: The file path of the test file cannot point to SRCDIR as
// specified in testpaths.h since this breaks 'make distcheck'

int main(int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd, STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Be patient, this test may take a few moments ..."
              << std::endl;

    return 1;
    // try {
    //     YAPET::Key key ("JustAPassword");
    //     YAPET::File file (FN, key, true);
    // assert(file.getFileVersion(key) == YAPET::VERSION_2);
    //     std::list<YAPET::PartDec> list;

    //     for (int i = 0; i < ROUNDS; i++) {
    //         progress();
    //         YAPET::Record<YAPET::PasswordRecord> record;
    //         YAPET::PasswordRecord *ptr_rec = record;
    //         snprintf ( (char*) ptr_rec->name, YAPET::NAME_SIZE, NAME, i);
    //         snprintf ( (char*) ptr_rec->host, YAPET::HOST_SIZE, HOST, i);
    //         snprintf ( (char*) ptr_rec->username, YAPET::USERNAME_SIZE,
    //         UNAME, i); snprintf ( (char*) ptr_rec->password,
    //         YAPET::PASSWORD_SIZE, PW, i); snprintf ( (char*)
    //         ptr_rec->comment, YAPET::COMMENT_SIZE, COMMENT, i);
    //         list.push_back (YAPET::PartDec (record, key) );
    //         // Commented out due to new progress indicator
    //         // std::cout << "\r";
    //     }

    //     file.save (list);
    // } catch (std::exception& ex) {
    //     std::cout << std::endl;
    //     std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
    //     return 1;
    // }

    // try {
    //     YAPET::Key key ("JustAPassword");
    //     YAPET::File file (FN, key, false);
    // assert(file.getFileVersion(key) == YAPET::VERSION_2);
    //     std::list<YAPET::PartDec> list = file.read (key);

    //     if (list.size() != ROUNDS) {
    //         std::cout << std::endl;
    //         std::cout << "List has unexpected size" << std::endl;
    //         return 1;
    //     }

    //     std::list<YAPET::PartDec>::iterator it = list.begin();

    //     for (int i = 0; it != list.end(); i++) {
    //         check_record (*it, key, i);
    //         it++;
    //     }
    // } catch (std::exception& ex) {
    //     std::cout << std::endl;
    //     std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
    //     return 1;
    // }

    // std::cout << std::endl;
    // return 0;
}
