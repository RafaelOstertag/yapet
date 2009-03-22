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

static const char* xterm_name = "xterm";
static const char* dtterm_name = "dtterm";

#ifdef CANSETTITLE
void setTerminalTitle (const std::string& title) {
#ifdef HAVE_STRNCASECMP
    if (strncasecmp(termname(), xterm_name, strlen(xterm_name)) == 0 ||
	strncasecmp(termname(), xterm_name, strlen(dtterm_name)) == 0) {
#elif HAVE_STRCMP
    if (strcmp(termname(), xterm_name, strlen(xterm_name)) == 0 ||
	strcmp(termname(), xterm_name, strlen(dtterm_name)) == 0) {
#else
# error "Sorry, strncasecmp or strcmp needed"
#endif
	fprintf(stdout, "%c]0;%s%c", '\033', title.c_str(), '\007');
	fflush(stdout);
    }
}
#endif // CANSETTITLE
