// $Id$
// Advanced tests for yapet/pwgen/charpool.*

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <cstring>
#include <typeinfo>

#include <unistd.h>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

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

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
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
	if (tmp1.numPoolsNotRead() != 3)
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
	if (tmp1.numPoolsNotRead() != 2)
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
	if (tmp1.numPoolsNotRead() != 1)
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
	if (tmp1.numPoolsNotRead() != 0)
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
