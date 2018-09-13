#include <unistd.h>
#include <cstdlib>

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) std::exit(1);

    std::exit(0);
}
