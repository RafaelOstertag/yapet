// $Id$

#include <typeinfo>
#include <iostream>

#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <bdbuffer.h>
#include <key.h>

#define CONTROL_STR "ABCDEFGHIJKLMNOPQRSTUVW"

int main (int argc, char** argv) {
    try {
        YAPET::Key key ("TEST");
        YAPET::Crypt crypt (key);
        YAPET::FileHeader header;
        header.version = 1;
        memcpy (header.control, CONTROL_STR, YAPET::HEADER_CONTROL_SIZE);
        YAPET::Record<YAPET::FileHeader> record (header);
        YAPET::BDBuffer* data = crypt.encrypt (record);
        YAPET::Record<YAPET::FileHeader>* dec_header;
        dec_header = crypt.decrypt<YAPET::FileHeader> (*data);
        delete data;
        YAPET::FileHeader* fh_ptr (*dec_header);

        if (memcmp (fh_ptr->control, CONTROL_STR, YAPET::HEADER_CONTROL_SIZE) != 0)
            return 1;

        delete dec_header;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
