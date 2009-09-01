// $Id$

#include <typeinfo>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <bdbuffer.h>
#include <key.h>

#define CONTROL_STR "ABCDEFGHIJKLMNOPQRSTUVW"

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        YAPET::Key key ("TEST");
        YAPET::Crypt crypt (key);
        YAPET::FileHeader_64 header;
        header.version = YAPET::VERSION_2;
        memcpy (header.control, CONTROL_STR, YAPET::HEADER_CONTROL_SIZE);
        YAPET::Record<YAPET::FileHeader_64> record (header);
        YAPET::BDBuffer* data = crypt.encrypt (record);
        YAPET::Record<YAPET::FileHeader_64>* dec_header;
        dec_header = crypt.decrypt<YAPET::FileHeader_64> (*data);
        delete data;
        YAPET::FileHeader_64* fh_ptr (*dec_header);

        if (memcmp (fh_ptr->control, CONTROL_STR, YAPET::HEADER_CONTROL_SIZE) != 0)
            return 1;

        delete dec_header;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;

}
