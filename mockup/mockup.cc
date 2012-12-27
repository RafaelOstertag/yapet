#include <unistd.h>
#include "curs.h"

int
main() {
    Curses::init();

    sleep(1);

    Curses::end();
    return 0;
}
