// $Id$
//
// Test behavior if maximum record usage.
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

const char* chars =
    "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789";

char NAMEV[YAPET::NAME_SIZE];
char HOSTV[YAPET::HOST_SIZE];
char USERV[YAPET::USERNAME_SIZE];
char PASSV[YAPET::PASSWORD_SIZE];
char COMMV[YAPET::COMMENT_SIZE];

void init_vars() {
    int len = std::strlen(chars);

    for (int i = 0; i < YAPET::NAME_SIZE - 1; i++) NAMEV[i] = chars[i % len];

    for (int i = 0; i < YAPET::HOST_SIZE - 1; i++) HOSTV[i] = chars[i % len];

    for (int i = 0; i < YAPET::USERNAME_SIZE - 1; i++)
        USERV[i] = chars[i % len];

    for (int i = 0; i < YAPET::PASSWORD_SIZE - 1; i++)
        PASSV[i] = chars[i % len];

    for (int i = 0; i < YAPET::COMMENT_SIZE - 1; i++) COMMV[i] = chars[i % len];

    NAMEV[YAPET::NAME_SIZE - 1] = 0;
    HOSTV[YAPET::HOST_SIZE - 1] = 0;
    USERV[YAPET::USERNAME_SIZE - 1] = 0;
    PASSV[YAPET::PASSWORD_SIZE - 1] = 0;
    COMMV[YAPET::COMMENT_SIZE - 1] = 0;
}

int main(int, char**) {
    init_vars();

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

    // progress();
    // YAPET::Record<YAPET::PasswordRecord> record;
    // YAPET::PasswordRecord *ptr_rec = record;
    // std::strncpy((char*) ptr_rec->name, NAMEV, YAPET::NAME_SIZE);
    // std::strncpy((char*) ptr_rec->host, HOSTV, YAPET::HOST_SIZE);
    // std::strncpy((char*) ptr_rec->username, USERV, YAPET::USERNAME_SIZE);
    // std::strncpy((char*) ptr_rec->password, PASSV, YAPET::PASSWORD_SIZE);
    // std::strncpy((char*) ptr_rec->comment, COMMV, YAPET::COMMENT_SIZE);

    // list.push_back (YAPET::PartDec (record, key) );

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

    //     if (list.size() != 1) {
    //         std::cout << std::endl;
    //         std::cout << "List has unexpected size" << std::endl;
    //         return 1;
    //     }

    //     std::list<YAPET::PartDec>::iterator it = list.begin();

    // const YAPET::BDBuffer& enc_rec = it->getEncRecord();

    // YAPET::Crypt crypt(key);
    // YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec =
    // crypt.decrypt<YAPET::PasswordRecord> (enc_rec);

    // YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

    // if (std::strcmp((char*) ptr_pw->name, NAMEV) != 0) {
    //         std::cerr << "Name does not match" << std::endl;
    //     delete ptr_dec_rec;
    //     return 1;
    // }

    // if (std::strcmp((char*) ptr_pw->host, HOSTV) != 0) {
    //         std::cerr << "Host does not match" << std::endl;
    //     delete ptr_dec_rec;
    //     return 1;
    // }

    // if (std::strcmp((char*) ptr_pw->username, USERV) != 0) {
    //         std::cerr << "User Name does not match" << std::endl;
    //     delete ptr_dec_rec;
    //     return 1;
    // }

    // if (std::strcmp((char*) ptr_pw->password, PASSV) != 0) {
    //         std::cerr << "Password does not match" << std::endl;
    //     delete ptr_dec_rec;
    //     return 1;
    // }

    // if (std::strcmp((char*) ptr_pw->comment, COMMV) != 0) {
    //         std::cerr << "Comment does not match" << std::endl;
    //     delete ptr_dec_rec;
    //     return 1;
    // }

    // delete ptr_dec_rec;
    // } catch (std::exception& ex) {
    //     std::cout << std::endl;
    //     std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
    //     return 1;
    // }

    // std::cout << std::endl;
    // return 0;
}
