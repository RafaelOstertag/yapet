// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <typeinfo>

#include <cstring>
#include <iostream>

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

#define NAME "Test name"
#define HOST "Test host"
#define UNAME "Test username"
#define PW "Test password"
#define COMMENT "Test comment"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        YAPET::Key key ("TEST");
        YAPET::Crypt crypt (key);
        YAPET::Record<YAPET::PasswordRecord> record;
        YAPET::PasswordRecord* ptr_rec = record;
        memcpy (ptr_rec->name, NAME, strlen (NAME) );
        memcpy (ptr_rec->host, HOST, strlen (HOST) );
        memcpy (ptr_rec->username, UNAME, strlen (UNAME) );
        memcpy (ptr_rec->password, PW, strlen (PW) );
        memcpy (ptr_rec->comment, COMMENT, strlen (COMMENT) );
        YAPET::PartDec pdec;
        pdec.setRecord (record, key);
        YAPET::Record<YAPET::PasswordRecord>* dec_rec = crypt.decrypt<YAPET::PasswordRecord> (pdec.getEncRecord() );
        YAPET::PasswordRecord* ptr_dec_rec = *dec_rec;
        int retval = memcmp (ptr_rec->name, ptr_dec_rec->name, YAPET::NAME_SIZE);

        if (retval != 0)
            return 1;

        retval = memcmp (ptr_rec->host, ptr_dec_rec->host, YAPET::HOST_SIZE);

        if (retval != 0)
            return 1;

        retval = memcmp (ptr_rec->username, ptr_dec_rec->username, YAPET::USERNAME_SIZE);

        if (retval != 0)
            return 1;

        retval = memcmp (ptr_rec->password, ptr_dec_rec->password, YAPET::PASSWORD_SIZE);

        if (retval != 0)
            return 1;

        retval = memcmp (ptr_rec->comment, ptr_dec_rec->comment, YAPET::COMMENT_SIZE);

        if (retval != 0)
            return 1;

        delete dec_rec;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
