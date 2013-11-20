// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

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

#include "record.h"

struct tmp {
    char v1[5];
    char v2[20];
};

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    tmp t;
    YAPET::Record<tmp> record (t);

    if (record.size() != 25) {
        return 1;
    }

    YAPET::Record<tmp> record2 (record);
    YAPET::Record<tmp> record3 (t);
    record3 = record2 = record;
    return 0;

}
