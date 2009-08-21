// $Id$
// Advanced tests for yapet/pwgen/charpool.*

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

#include <charpool.h>

#include "testpaths.h"

using namespace YAPET::PWGEN;


const char t_letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char t_digits[] = "0123456789";
const char t_punct[] = ".,;:-!?'";
const char t_special[] = "_+\"*%&/()[]={}<>";
const char t_other[] = "§°@#\\|$£~`^";

int main (int, char**) {
    std::cout << std::endl;

    try {
        CharacterPool tmp1 (LETTERS | PUNCT | OTHER);

	if (tmp1.hadPoolReads(LETTERS)) 
	    return 1;
	if (tmp1.hadPoolReads(PUNCT))
	    return 1;
	if (tmp1.hadPoolReads(OTHER))
	    return 1;

	if (tmp1.getPoolsWithRead() != 0)
	    return 1;
	if (tmp1.getNumPoolsNotRead() != 3)
	    return 1;

	size_t start;
	size_t len;
	len =tmp1.getPoolPos(LETTERS, &start);
	if ( len != strlen(t_letters) )
	    return 1;
	if (tmp1[start] != t_letters[0])
	    return 1;
	if (tmp1[start+len-1] != t_letters[strlen(t_letters)-1])
	    return 1;
	if (!tmp1.hadPoolReads(LETTERS))
	    return 1;
	if (tmp1.getNumPoolsNotRead() != 2)
	    return 1;

	len = tmp1.getPoolPos(PUNCT, &start);
	if ( len != strlen(t_punct) )
	    return 1;
	if (tmp1[start] != t_punct[0])
	    return 1;
	if (tmp1[start+len-1] != t_punct[strlen(t_punct)-1])
	    return 1;
	if (!tmp1.hadPoolReads(PUNCT))
	    return 1;
	if (tmp1.getNumPoolsNotRead() != 1)
	    return 1;
	
	len = tmp1.getPoolPos(OTHER, &start);
	if ( len != strlen(t_other) )
	    return 1;
	if (tmp1[start] != t_other[0])
	    return 1;
	if (tmp1[start+len-1] != t_other[strlen(t_other)-1])
	    return 1;
	if (!tmp1.hadPoolReads(OTHER))
	    return 1;
	if (tmp1.getNumPoolsNotRead() != 0)
	    return 1;

	len = tmp1.getPoolPos(DIGITS, &start);
	if (! (start == 0 && len == 0))
	    return 1;
	len = tmp1.getPoolPos(SPECIAL, &start);
	if (! (start == 0 && len == 0))
	    return 1;
	    

    } catch (std::exception& ex) {
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
