// $Id$
// Simple tests for yapet/pwgen/pwgen.*

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <typeinfo>

#include <pwgen.h>

#include "testpaths.h"

using namespace YAPET::PWGEN;

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
        // The the constructors
        PWGen pwgen1 (ALL);
        PWGen pwgen2 (pwgen1);
        PWGen pwgen3 (LETTERS);
        PWGen pwgen4 (DIGITS);
        pwgen4 = pwgen3;


	// Test switching of charpools and rng
	PWGen *pwgen5 = new PWGen(ALL);
	pwgen5->setNewPool(LETTERS);
	pwgen5->setNewRNG(RAND); // Will fail if RAND is not available, but I
				 // hardly doubt that RAND is not available
	delete pwgen5;

        PWGen *pwgen;
	//
	// Make sure we don't waste /dev/random if possible
	//
	YAPET::PWGEN::RNGENGINE rngengine;
#ifdef HAVE__DEV_RANDOM
# ifdef HAVE__DEV_URANDOM
	rngengine = DEVURANDOM;
# else
#  ifdef HAVE_LRAND48
	rngengine = LRAND48;
#  else
#   ifdef HAVE_RAND
	rngengine = RAND;
#   else
#    error "NO PASSWORD GENERATOR FOUND ON SYSTEM"
#   endif
#  endif
# endif
#else
	rngengine = AUTO;
#endif
        // Loop for iterating the password length
        for (int size = 1; size <= 20; size++) {
            std::cout << " ==> LETTERS (size " << size << "): " ;
            pwgen = new PWGen (LETTERS,  rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif

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
            pwgen = new PWGen (DIGITS, rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif
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
            pwgen = new PWGen (PUNCT, rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif
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
            pwgen = new PWGen (SPECIAL, rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif
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
            pwgen = new PWGen (OTHER, rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif
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
            pwgen = new PWGen (ALL, rngengine);
#if defined(HAVE__DEV_RANDOM) && (defined(HAVE__DEV_URANDOM) || defined(HAVE_LRAND48) || defined(HAVE_RAND))
	    assert(pwgen->getRNGUsed() != DEVRANDOM);
#endif
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

