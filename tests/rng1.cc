// $Id$
// Simple tests for yapet/pwgen/rng.*

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <typeinfo>

#include <rng.h>

#include "testpaths.h"

using namespace YAPET::PWGEN;

int main (int, char**) {
    std::cout << std::endl;

    try {
        // The the constructors
        RNG rng1;
        RNG tmp1 (rng1);
        RNG tmp2 = rng1;
        RNG tmp3 = tmp1;
        RNG tmp4;
        RNG tmp5;
        tmp5 = tmp4;
#ifdef HAVE__DEV_RANDOM
        RNG devrandom (DEVRANDOM);
#endif
#ifdef HAVE__DEV_URANDOM
        RNG devurandom (DEVURANDOM);
#endif
#ifdef HAVE_LRAND48
        RNG _lrand48 (LRAND48);
#endif
#ifdef HAVE_RAND
        RNG _rand (RAND);
#endif

        try {
            RNG rng6 (NONE);
            return 1;
        } catch (PWGenRNGNotAvailable&) {
            // OK
        } catch (...) {
            return 1;
        }

#define MYCEILING 20
#ifdef HAVE__DEV_RANDOM
# ifdef DEBUG
        std::cout << " ==> /dev/random: ";

        for (int i = 0; i < MYCEILING; i++)
            std::cout << devrandom (MYCEILING) << " ";
# else
	std::cout << " ==> /dev/random will only be tested in debug mode.";
# endif
        std::cout << std::endl;

#endif
#ifdef HAVE__DEV_URANDOM
        std::cout << " ==> /dev/urandom: ";

        for (int i = 0; i < MYCEILING; i++)
            std::cout << devurandom (MYCEILING) << " ";

        std::cout << std::endl;
#endif
#ifdef HAVE_LRAND48
        std::cout << " ==> lrand48(): ";

        for (int i = 0; i < MYCEILING; i++)
            std::cout << _lrand48 (MYCEILING) << " ";

        std::cout << std::endl;
#endif
#ifdef HAVE_RAND
        std::cout << " ==> rand(): ";

        for (int i = 0; i < MYCEILING; i++)
            std::cout << _rand (MYCEILING) << " ";

        std::cout << std::endl;
#endif
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
