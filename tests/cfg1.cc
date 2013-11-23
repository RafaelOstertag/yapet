// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <cstdlib>

#include "cfg.h"
#include "testpaths.h"

#define MYEXIT(x) { std::cerr << x << std::endl; std::exit(1); }

using namespace YAPET;
using namespace YAPET::CONFIG;

void
test_initvalues() {
    // Test default values
    Config cfg;

    if (!cfg.petfile.get().empty())
	MYEXIT("cfg.petfile not empty");

    if (cfg.timeout != Consts::def_locktimeout)
	MYEXIT("locktimeout mismatch");

    if (cfg.filesecurity != Consts::def_filesecurity)
	MYEXIT("filesecurity mismatch");

    if (cfg.pwgenpwlen != Consts::def_pwlen)
	MYEXIT("pwgenpwlen mismatch");

    if (cfg.pwgen_rng != Consts::def_pwgen_rng)
	MYEXIT("pwgen_rng mismatch");

    if (cfg.pwgen_letters!=PWGEN::HAS_LETTERS(Consts::def_character_pools))
	MYEXIT("pwgen_letters mismatch");

    if (cfg.pwgen_digits!=PWGEN::HAS_DIGITS(Consts::def_character_pools))
	MYEXIT("pwgen_digits mismatch");

    if (cfg.pwgen_punct!=PWGEN::HAS_PUNCT(Consts::def_character_pools))
	MYEXIT("pwgen_punct mismatch");

    if (cfg.pwgen_special!=PWGEN::HAS_SPECIAL(Consts::def_character_pools))
	MYEXIT("pwgen_special mismatch");

    if (cfg.pwgen_other!=PWGEN::HAS_OTHER(Consts::def_character_pools))
	MYEXIT("pwgen_other mismatch");

    if (cfg.character_pools() != Consts::def_character_pools)
	MYEXIT("character_pools() mismatch");

    if (cfg.allow_lock_quit != Consts::def_allow_lock_quit)
	MYEXIT("allow_lock_quit mismatch");

    if (cfg.pw_input_timeout != Consts::def_pw_input_timeout)
	MYEXIT("pw_input_timeout mismatch");
}

void
test_index_operator() {
    Config cfg;

    try {
	if (!dynamic_cast<CfgValPetFile&>(cfg["load"]).get().empty())
	    MYEXIT("cfg.petfile not empty");
	
	if (dynamic_cast<CfgValInt&>(cfg["locktimeout"]) != Consts::def_locktimeout)
	    MYEXIT("locktimeout mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["checkfsecurity"]) != Consts::def_filesecurity)
	    MYEXIT("filesecurity mismatch");
	
	if (dynamic_cast<CfgValInt&>(cfg["pwgen_pwlen"]) != Consts::def_pwlen)
	    MYEXIT("pwgenpwlen mismatch");
	
	if (dynamic_cast<CfgValRNG&>(cfg["pwgen_rng"]) != Consts::def_pwgen_rng)
	    MYEXIT("pwgen_rng mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["pwgen_letters"]) !=PWGEN::HAS_LETTERS(Consts::def_character_pools))
	    MYEXIT("pwgen_letters mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["pwgen_digits"])!=PWGEN::HAS_DIGITS(Consts::def_character_pools))
	    MYEXIT("pwgen_digits mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["pwgen_punct"])!=PWGEN::HAS_PUNCT(Consts::def_character_pools))
	    MYEXIT("pwgen_punct mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["pwgen_special"])!=PWGEN::HAS_SPECIAL(Consts::def_character_pools))
	    MYEXIT("pwgen_special mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["pwgen_other"])!=PWGEN::HAS_OTHER(Consts::def_character_pools))
	    MYEXIT("pwgen_other mismatch");
	
	if (dynamic_cast<CfgValBool&>(cfg["allowlockquit"]) != Consts::def_allow_lock_quit)
	    MYEXIT("allow_lock_quit mismatch");
	
	if (dynamic_cast<CfgValInt&>(cfg["pwinputtimeout"]) != Consts::def_pw_input_timeout)
	    MYEXIT("pw_input_timeout mismatch");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg[""].set_str("");
	MYEXIT("Config::operator[] allowed \"\"");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["       "].set_str("");
	MYEXIT("Config::operator[] allowed \"         \"");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["__should_not_be_found"].set_str("");
	MYEXIT("Config::operator[] allowed \"__should_not_be_found\"");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["   locktimeout"].set_str("50");
	if (cfg.timeout!=50)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["locktimeout                "].set_str("51");
	if (cfg.timeout!=51)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["                locktimeout                "].set_str("52");
	if (cfg.timeout!=52)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg[" locktimeout"].set_str("53");
	if (cfg.timeout!=53)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg["locktimeout "].set_str("54");
	if (cfg.timeout!=54)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	cfg[" locktimeout "].set_str("55");
	if (cfg.timeout!=55)
	    MYEXIT("Config::operator does[] not properly set value");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }
}

int main (int, char**) {

    test_initvalues();

    test_index_operator();

    return 0;

}
