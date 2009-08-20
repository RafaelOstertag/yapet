// $Id$

#include "cfgfile.h"
#include "testpaths.h"
#include <typeinfo>
#include <stdio.h>
#include <iostream>
#include <exception>

using namespace YAPET::CONFIG;

int main (int, char**) {
    std::cout << std::endl;

    try {
	std::cout << " ==> Reading file " SRCDIR "/cfgfile1.rc" << std::endl;
	ConfigFile cfgfile1( SRCDIR "/cfgfile1.rc" );
	if (!cfgfile1.isOpenSuccess())
	    return 1;
	if (cfgfile1.getFileToLoad() != "test.pet")
	    return 1;
	if (cfgfile1.getIgnoreRC() != true)
	    return 1;
	if (cfgfile1.getLockTimeout() != 20)
	    return 1;
	if (cfgfile1.getUseFileSecurity() != true)
	    return 1;
	if (cfgfile1.getPWGenPWLen() != 14)
	    return 1;
	if (cfgfile1.getPWGenLetters() != true)
	    return 1;
	if (cfgfile1.getPWGenDigits() != true)
	    return 1;
	if (cfgfile1.getPWGenPunct() != true)
	    return 1;
	if (cfgfile1.getPWGenSpecial() != true)
	    return 1;
	if (cfgfile1.getPWGenOther() != true)
	    return 1;

	std::cout << " ==> Reading file " SRCDIR "/cfgfile2.rc" << std::endl;
	ConfigFile cfgfile2( SRCDIR "/cfgfile2.rc" );
	if (!cfgfile2.isOpenSuccess())
	    return 1;
	if (cfgfile2.getFileToLoad() != "test.pet")
	    return 1;
	if (cfgfile2.getIgnoreRC() != false)
	    return 1;
	if (cfgfile2.getLockTimeout() != 21)
	    return 1;
	if (cfgfile2.getUseFileSecurity() != false)
	    return 1;
	if (cfgfile2.getPWGenPWLen() != 15)
	    return 1;
	if (cfgfile2.getPWGenLetters() != false)
	    return 1;
	if (cfgfile2.getPWGenDigits() != true)
	    return 1;
	if (cfgfile2.getPWGenPunct() != false)
	    return 1;
	if (cfgfile2.getPWGenSpecial() != true)
	    return 1;
	if (cfgfile2.getPWGenOther() != false)
	    return 1;
    } catch (std::exception& ex) {
		std::cout << " ==> " << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
