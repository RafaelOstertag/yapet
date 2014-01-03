// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "record.h"

struct test_struct {
    char v1[5];
    char v2[20];
};

struct test_struct2 {
	char v1[8];
	char v2[8];
};

test_struct2 pre_initialized = {
    "1234567",
    "abcdefg"
};

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    test_struct t;
    YAPET::Record<test_struct> record (t);

    if (record.size() != 25) {
        return 1;
    }

    YAPET::Record<test_struct> record2 (record);
    // Yes, t is used uninitialized, but it will be initialized thru
    // record3
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif
    YAPET::Record<test_struct> record3 (t);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    record3 = record2 = record;

    YAPET::Record<test_struct2> record4(pre_initialized);

    if (std::strcmp(static_cast<test_struct2*>(record4)->v1, pre_initialized.v1)!=0)
	return 1;

    if (std::strcmp(static_cast<test_struct2*>(record4)->v2, pre_initialized.v2)!=0)
	return 1;

    if (std::strcmp(static_cast<test_struct2*>(record4)->v1, pre_initialized.v2)==0)
	return 1;

    if (std::strcmp(static_cast<test_struct2*>(record4)->v2, pre_initialized.v1)==0)
	return 1;

    return 0;

}
