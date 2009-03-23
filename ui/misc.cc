// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008,2009  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "misc.h"

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#ifdef HAVE_STDIO_H
#include <stdio.h>
#else
# error "Sorry, stdio.h needed"
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

// For convenience
#if defined(HAVE_TERMINALTITLE) && defined(HAVE_TERMNAME)
#define CANSETTITLE
#endif

/**
 * @brief The terminals we know
 *
 * Holds the terminals we know that can have set the title.
 */
static const char* xterms[] = {
    "xterm",
    "dtterm",
    NULL
};

/**
 * @brief Wrapper function
 *
 * Wrapper function for strncasecmp, strncmp, strcmp.
 *
 * @param s1 String one
 * @param s2 String two
 */
inline static int mystrcmp(const char* s1, const char* s2) {
#ifdef HAVE_STRNCASECMP
    return strncasecmp(s1, s2, (strlen(s1)<strlen(s2) ? strlen(s1) : strlen(s2)) );
#elif HAVE_STRNCMP
    return strncmp(s1, s2, (strlen(s1)<strlen(s2) ? strlen(s1) : strlen(s2)) );
#elif HAVE_STRCMP
    return strcmp(s1, s2);
#else
# error "Sorry, strncasecmp, strncmp, or strcmp needed"
#endif
}



/**
 * Indicates whether or not the terminal is an X Terminal. It uses the
 * \c termname() function to determine the type of terminal. It compares the
 * return value of \c termname() with the values stored in \c xterms.
 *
 * The function is always available, but always returns \c false if \c termname
 * is not supported
 *
 * @retval \c true if the terminal is an X Terminal, \c false otherwise.
 */
#ifdef HAVE_TERMNAME
bool isXTerm() {
    const char** tmp = xterms;
    char* tn = termname();
    while (*tmp != NULL) {
	if (mystrcmp(tn,*tmp) == 0) return true;
	tmp++;
    }
    return false;
}
#else
inline bool isXTerm() { return false; }
#endif // HAVE_TERMNAME

/**
 * If supported, set the title on a terminal. The function is always available,
 * but might not have an effect.
 *
 * @param title the title to set
 */
#ifdef CANSETTITLE
void setTerminalTitle (const std::string& title) {
    if (isXTerm())
	fprintf(stdout, "%c]0;%s%c", '\033', title.c_str(), '\007');
	fflush(stdout);
    }
}
#else // CANSETTITLE
inline void setTerminalTitle (const std::string& title) {}
#endif // CANSETTITLE
