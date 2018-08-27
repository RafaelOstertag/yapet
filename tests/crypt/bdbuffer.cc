// $Id$

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

#include <bdbuffer.h>

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        YAPET::BDBuffer data1;

        if (data1.size() != 0)
            return 1;

        YAPET::BDBuffer data2 (512);

        if (data2.size() != 512)
            return 1;

        YAPET::BDBuffer data3 (1024);

        if (data3.size() != 1024)
            return 1;

        data1 = data2;

        if (data1.size() != 512)
            return 1;

        data2 = data3;

        if (data2.size() != 1024)
            return 1;

        data3 = data1;

        if (data3.size() != 512)
            return 1;

        data3.resize (2);

        if (data3.size() != 2)
            return 1;

        data3.resize (2048);

        if (data3.size() != 2048)
            return 1;

        data3.resize (1);

        if (data3.size() != 1)
            return 1;

        data3.resize (100);
        data3[0] = 'a';
        data3[1] = 'b';
        data3[2] = 'c';
        data3[3] = '\0';
        uint8_t* tmp = data3;

        if (std::strcmp ( (char*) tmp, "abc") != 0)
            return 1;

        try {
            data3.at (100);
        } catch (std::out_of_range& ex_) {
            // Everything fine...
            std::cout << " ==> " << typeid (ex_).name() << ": " << ex_.what() << " (OK) " << std::endl;
        }
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;

}