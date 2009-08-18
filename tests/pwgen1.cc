// $Id$
// Simple tests for yapet/pwgen/pwgen.*

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include <typeinfo>

#include <pwgen.h>

#include "testpaths.h"

using namespace YAPET::PWGEN;

int main (int, char**) {
    try {
        // The the constructors
        PWGen pwgen1 (ALL);
        PWGen pwgen2 (pwgen1);
        PWGen pwgen3 (LETTERS);
        PWGen pwgen4 (DIGITS);
        pwgen4 = pwgen3;
        PWGen *pwgen;

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> LETTERS (size " << size << "): " ;
            pwgen = new PWGen (LETTERS);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> DIGITS (size " << size << "): " ;
            pwgen = new PWGen (DIGITS);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> PUNCT (size " << size << "): " ;
            pwgen = new PWGen (PUNCT);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> SPECIAL (size " << size << "): " ;
            pwgen = new PWGen (SPECIAL);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> OTHER (size " << size << "): " ;
            pwgen = new PWGen (OTHER);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }

        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> ALL (size " << size << "): " ;
            pwgen = new PWGen (ALL);

            for (int i = 0; i < 5; i++) {
                pwgen->generatePassword (size);
                std::cout << pwgen->getPassword() << " ";
            }

            std::cout << std::endl;
            delete pwgen;
        }
    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
