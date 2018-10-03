// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <typeinfo>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "cfg.h"
#include "testpaths.h"

using namespace YAPET::CONFIG;

int main(int, char**) {
    Config cfg;

    try {
        ConfigFile cfgfile1(cfg, SRCDIR "/cfgfile1.rc");
        cfgfile1.parse();

        if (cfg.timeout != 20) {
            std::cerr << "locktimeout does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.pw_input_timeout != 30) {
            std::cerr << "pwinputtimeout does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.allow_lock_quit) {
            std::cerr << "allowlockquit does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.filesecurity) {
            std::cerr << "checkfsecurity does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.petfile.get() != "test.pet") {
            std::cerr << "load does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.pwgenpwlen != 14) {
            std::cerr << "pwgen_pwlen does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_letters) {
            std::cerr << "pwgen_letters does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_digits) {
            std::cerr << "pwgen_digits does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_punct) {
            std::cerr << "pwgen_punct does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_special) {
            std::cerr << "pwgen_special does not match (#1)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_other) {
            std::cerr << "pwgen_other does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.pwgen_rng != yapet::pwgen::DEVURANDOM) {
            std::cerr << "pwgen_rng does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.argon2_iterations != 84) {
            std::cerr << "argon2_iterations does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.argon2_memory != 84000) {
            std::cerr << "argon2_memory does not match (#1)" << std::endl;
            abort();
        }

        if (cfg.argon2_parallelism != 42) {
            std::cerr << "argon2_parallelism does not match (#1)" << std::endl;
            abort();
        }

        //
        // test 2
        //
        ConfigFile cfgfile2(cfg, SRCDIR "/cfgfile2.rc");
        cfgfile2.parse();

        if (cfg.timeout != 21) {
            std::cerr << "locktimeout does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pw_input_timeout != 25) {
            std::cerr << "pwinputtimeout does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.allow_lock_quit) {
            std::cerr << "allowlockquit does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.filesecurity) {
            std::cerr << "checkfsecurity does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.petfile.get() != "/test/test2.pet") {
            std::cerr << "load does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pwgenpwlen != 15) {
            std::cerr << "pwgen_pwlen does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pwgen_letters) {
            std::cerr << "pwgen_letters does not match (#2)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_digits) {
            std::cerr << "pwgen_digits does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pwgen_punct) {
            std::cerr << "pwgen_punct does not match (#2)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_special) {
            std::cerr << "pwgen_special does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pwgen_other) {
            std::cerr << "pwgen_other does not match (#2)" << std::endl;
            abort();
        }

        if (cfg.pwgen_rng != yapet::pwgen::RAND) {
            std::cerr << "pwgen_rng does not match (#2)" << std::endl;
            abort();
        }

        //
        // test 3
        //
        ConfigFile cfgfile3(cfg, SRCDIR "/cfgfile3.rc");
        cfgfile3.parse();

        if (cfg.timeout != 23) {
            std::cerr << "locktimeout does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pw_input_timeout != 24) {
            std::cerr << "pwinputtimeout does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.allow_lock_quit) {
            std::cerr << "allowlockquit does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.filesecurity) {
            std::cerr << "checkfsecurity does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.petfile.get() != "/test/with space/test2.pet") {
            std::cerr << "load does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pwgenpwlen != 19) {
            std::cerr << "pwgen_pwlen does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pwgen_letters) {
            std::cerr << "pwgen_letters does not match (#3)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_digits) {
            std::cerr << "pwgen_digits does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pwgen_punct) {
            std::cerr << "pwgen_punct does not match (#3)" << std::endl;
            abort();
        }

        if (!cfg.pwgen_special) {
            std::cerr << "pwgen_special does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pwgen_other) {
            std::cerr << "pwgen_other does not match (#3)" << std::endl;
            abort();
        }

        if (cfg.pwgen_rng != yapet::pwgen::RAND) {
            std::cerr << "pwgen_rng does not match (#3)" << std::endl;
            abort();
        }

    } catch (std::exception& ex) {
        std::cout << " ==> " << typeid(ex).name() << ": " << ex.what()
                  << std::endl;
        abort();
    }

    exit(0);
}
