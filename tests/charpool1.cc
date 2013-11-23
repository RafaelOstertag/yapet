// $Id$
// Simple tests for yapet/pwgen/charpool.*

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <cstring>
#include <typeinfo>

#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "charpool.h"

#include "testpaths.h"

using namespace YAPET::PWGEN;


const char t_letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char t_digits[] = "0123456789";
const char t_punct[] = ".,;:-!?'";
const char t_special[] = "_+\"*%&/()[]={}<>";
const char t_other[] = "@#\\|$~`^";

class gainaccess : public CharacterPool {
    public:
        gainaccess (SUBPOOLS p) throw (std::runtime_error) : CharacterPool (p) { }
        const char* _getPool() {
            return getPool();
        }
};

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd ,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        // The the constructors
        CharacterPool tmp1 (LETTERS | DIGITS | PUNCT | SPECIAL | OTHER);
        CharacterPool tmp2 = tmp1;
        CharacterPool tmp3 (tmp1);
        gainaccess tmp4 (LETTERS);

        if ( (tmp4.getPoolLength() != std::strlen (t_letters) ) ||
                std::strncmp (tmp4._getPool(), t_letters, tmp4.getPoolLength() ) != 0 ) {
            std::cout << " ==> Pool LETTERS looks odd" << std::endl;
            return 1;
        }

        gainaccess tmp5 (DIGITS);

        if ( (tmp5.getPoolLength() != std::strlen (t_digits) ) ||
                std::strncmp (tmp5._getPool(), t_digits, tmp5.getPoolLength() ) != 0 ) {
            std::cout << " ==> Pool DIGITS looks odd" << std::endl;
            return 1;
        }

        gainaccess tmp6 (PUNCT);

        if ( (tmp6.getPoolLength() != std::strlen (t_punct) ) ||
                std::strncmp (tmp6._getPool(), t_punct, tmp6.getPoolLength() ) != 0 ) {
            std::cout << " ==> Pool PUNCT looks odd" << std::endl;
            return 1;
        }

        gainaccess tmp7 (SPECIAL);

        if ( (tmp7.getPoolLength() != std::strlen (t_special) ) ||
                std::strncmp (tmp7._getPool(), t_special, tmp7.getPoolLength() ) != 0 ) {
            std::cout << " ==> Pool SPECIAL looks odd" << std::endl;
            return 1;
        }

        gainaccess tmp8 (OTHER);

        if ( (tmp8.getPoolLength() != std::strlen (t_other) ) ||
                std::strncmp (tmp8._getPool(), t_other, tmp8.getPoolLength() ) != 0 ) {
            std::cout << " ==> Pool OTHER looks odd" << std::endl;
            return 1;
        }

        std::cout << " ==> ";
        CharacterPool* cp = new CharacterPool (LETTERS);

        for (size_t i = 0; i < cp->getPoolLength(); i++) {
            char c = (*cp) [i];

            if (c == 0) {
                std::cout << std::endl << " ==> Zero encountered at position " << i << std::endl;
                return 1;
            }

            std::cout << c;
        }

        delete cp;
        std::cout << std::endl;
        std::cout << " ==> ";
        cp = new CharacterPool (DIGITS);

        for (size_t i = 0; i < cp->getPoolLength(); i++) {
            char c = (*cp) [i];

           if (c == 0) {
                std::cout << std::endl << " ==> Zero encountered at position " << i << std::endl;
                return 1;
            }

            std::cout << c;
        }

        delete cp;
        std::cout << std::endl;
        std::cout << " ==> ";
        cp = new CharacterPool (PUNCT);

        for (size_t i = 0; i < cp->getPoolLength(); i++) {
            char c = (*cp) [i];

            if (c == 0) {
                std::cout << std::endl << " ==> Zero encountered at position " << i << std::endl;
                return 1;
            }

            std::cout << c;
        }

        delete cp;
        std::cout << std::endl;
        std::cout << " ==> ";
        cp = new CharacterPool (SPECIAL);

        for (size_t i = 0; i < cp->getPoolLength(); i++) {
            char c = (*cp) [i];

            if (c == 0) {
                std::cout << std::endl << " ==> Zero encountered at position " << i << std::endl;
                return 1;
            }

            std::cout << c;
        }

        delete cp;
        std::cout << std::endl;
        std::cout << " ==> ";
        cp = new CharacterPool (OTHER);

        for (size_t i = 0; i < cp->getPoolLength(); i++) {
            char c = (*cp) [i];

            if (c == 0) {
                std::cout << std::endl << " ==> Zero encountered at position " << i << std::endl;
                return 1;
            }

            std::cout << c;
        }

        delete cp;
        std::cout << std::endl;
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;

}
