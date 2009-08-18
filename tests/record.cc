// $Id$

#include <record.h>

struct tmp {
    char v1[5];
    char v2[20];
};

int main (int argc, char** argv) {
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
