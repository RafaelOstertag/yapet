// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <cstdlib>

#include "cfg.h"
#include "testpaths.h"

#define MYEXIT(x) { std::cerr << x << std::endl; abort(); }

using namespace YAPET;
using namespace YAPET::CONFIG;

template<class T>
class CfgValTest : public CfgVal<T> {
    public:
	CfgValTest(const T& v) : CfgVal<T>(v) {}
	CfgValTest(const CfgValTest<T>& c) : CfgVal<T>(c) {}
	CfgValTest& operator=(const CfgValTest<T>& c) {
	    CfgVal<T>::operator=(c);
	    return *this;
	}
	void set_str(const std::string&) {
	    abort();
	}
};

void test_val() {

    CfgValTest<int> testval(0);


    if (testval != 0)
	MYEXIT("CfgVal: improperly set (#1)");

    testval.set(1);

    if (testval != 1)
	MYEXIT("CfgVal: improperly set (#2)");

    testval.lock();

    testval.set(3);
    if (testval == 3)
	MYEXIT("CfgVal: lock not working (#1)");

    testval.unlock();
    testval.set(3);

    if (testval != 3)
	MYEXIT("CfgVal: unlock not working (#2)");

    CfgValTest<int> testval2(testval);

    if (testval2 != 3)
	MYEXIT("CfgVal: copy constructor not working");

    CfgValTest<int> testval3 = CfgValTest<int>(5);

    if (testval3 != 5)
	MYEXIT("CfgVal: operator=() not working");
	
}


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
	cfg[" locktimeout "].set_str("53");
	MYEXIT("Config::operator[] should not find \" locktimeout \"");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }
}

void
test_bool_val() {
    CfgValBool val(true);

    if (val!=true)
	MYEXIT("CfgValBool initialization failed");

    CfgValBool val2(false);
    if (val2!=false)
	MYEXIT("CfgValBool initialization failed");

    CfgValBool val3(true);

    val3.set_str("  FaLse ");
    if (val3!=false)
	MYEXIT("CfgValBool: setting 'FaLse' failed");

    val3.set_str("  True ");
    if (val3!=true)
	MYEXIT("CfgValBool: setting 'True' failed");

    val3.set_str("  0 ");
    if (val3!=false)
	MYEXIT("CfgValBool: setting '0' failed");

    val3.set_str("  1 ");
    if (val3!=true)
	MYEXIT("CfgValBool: setting '1' failed");

    val3.set_str("no");
    if (val3!=false)
	MYEXIT("CfgValBool: setting 'no' failed");

    val3.set_str("yes ");
    if (val3!=true)
	MYEXIT("CfgValBool: setting 'yes ' failed");

    val3.set_str(" disable");
    if (val3!=false)
	MYEXIT("CfgValBool: setting ' disable' failed");

    val3.set_str("enable");
    if (val3!=true)
	MYEXIT("CfgValBool: setting ' enable' failed");

    val3.set_str("disabled");
    if (val3!=false)
	MYEXIT("CfgValBool: setting 'disabled' failed");

    val3.set_str("enabled");
    if (val3!=true)
	MYEXIT("CfgValBool: setting 'enabled' failed");

    try {
	val3.set_str("enbld");
	MYEXIT("CfgValBool allowed 'enbld'");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	val3.set_str("e n a b l e d");
	MYEXIT("CfgValBool allowed 'e n a b l e d'");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }
}

void
test_petfile_val() {
    CfgValPetFile val1;

    if (!val1.get().empty())
	MYEXIT("CfgValPetFile: not empty");

    CfgValPetFile val2("// bla//bl //test");

    if (val2.get() != std::string("/ bla/bl /test.pet"))
	MYEXIT("CfgValPetFile: not initialized properly:" << val2.get());

    val2.set("");
    if (!val2.get().empty())
	MYEXIT("CfgValPetFile: cannot set empty pet file");

    val2.set("/test/t.pet");
    if (val2.get() != std::string("/test/t.pet"))
	MYEXIT("CfgValPetFile: value not set properly (#1)"); 

    val2.set("t");
    if (val2.get() != std::string("t.pet"))
	MYEXIT("CfgValPetFile: value not set properly (#2)");

    val2.set("te");
    if (val2.get() != std::string("te.pet"))
	MYEXIT("CfgValPetFile: value not set properly (#3)");

    val2.set("tes");
    if (val2.get() != std::string("tes.pet"))
	MYEXIT("CfgValPetFile: value not set properly (#4)");

    val2.set("test");
    if (val2.get() != std::string("test.pet"))
	MYEXIT("CfgValPetFile: value not set properly (#5)");

}

void
test_rng_val() {
    CfgValRNG val1;

    if (val1.get() != YAPET::PWGEN::AUTO)
	MYEXIT("CfgValRNG: not properly initialized");

    try {
	val1.set_str("dev urandom");
	if (val1.get() != YAPET::PWGEN::DEVURANDOM)
	    MYEXIT("CfgValRNG: value not properly set");
    } catch (std::exception& e) {
	MYEXIT(e.what());
	abort();
    }

    try {
	val1.set_str("evurandom");
	MYEXIT("CfgValRNG: allowed invalid argument");
    } catch (std::invalid_argument&) {
	// OK
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    
    try {
	val1.set_str("devrandom");
	if (val1 != YAPET::PWGEN::DEVRANDOM)
	    MYEXIT("CfgValRNG: value not properly set (#1)");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	val1.set_str("devurandom");
	if (val1 != YAPET::PWGEN::DEVURANDOM)
	    MYEXIT("CfgValRNG: value not properly set (#2)");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	val1.set_str("lrand48");
	if (val1 != YAPET::PWGEN::LRAND48)
	    MYEXIT("CfgValRNG: value not properly set (#3)");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

    try {
	val1.set_str("rand");
	if (val1 != YAPET::PWGEN::RAND)
	    MYEXIT("CfgValRNG: value not properly set (#4)");
    } catch (std::exception& e) {
	MYEXIT(e.what());
    }

}

void
test_int_val() {
    CfgValInt val1;

    if (val1 != 0)
	MYEXIT("CfgIntVal: not initialized properly");

    CfgValInt val2(4, -1, -1, 0);

    if (val2 != -1)
	MYEXIT("CfgIntVal: boundaries not working (#1)");

    CfgValInt val3(4,-2,-4,4);
    if (val3 != 4)
	MYEXIT("CfgIntVal: boundaries not wokring (#2)");

    CfgValInt val4(4,-2,-2,2);
    if (val4 != -2)
	MYEXIT("CfgIntVal: boundaries not wokring (#3)");


    CfgValInt val5(0,1,-2,2);
    val5.set(-4);

    if (val5 != 1)
	MYEXIT("CfgIntVal: boundaries not working (#4)");
	    
    val5.set(-2);
    if (val5 != -2)
	MYEXIT("CfgIntVal: boundaries not working (#5)");

    val5.set(-1);
    if (val5 != -1)
	MYEXIT("CfgIntVal: boundaries not working (#6)");

    val5.set(0);
    if (val5 != 0)
	MYEXIT("CfgIntVal: boundaries not working (#7)");

    val5.set(1);
    if (val5 != 1)
	MYEXIT("CfgIntVal: boundaries not working (#8)");

    val5.set(2);
    if (val5 != 2)
	MYEXIT("CfgIntVal: boundaries not working (#10)");

    val5.set(3);
    if (val5 != 1)
	MYEXIT("CfgIntVal: boundaries not working (#11)");
}

    

int main (int, char**) {

    test_val();

    test_bool_val();

    test_petfile_val();

    test_rng_val();

    test_int_val();

    test_initvalues();

    test_index_operator();

    return 0;

}
