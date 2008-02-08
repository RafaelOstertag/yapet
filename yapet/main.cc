// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

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

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif

#ifdef HAVE_IOSTREAM
# include <iostream>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#include "colors.h"
#include "mainwindow.h"

const char COPYRIGHT[] = "YAPET -- Yet Another Password Encryption Tool\n" \
    "Copyright (C) 2008  Rafael Ostertag\n"				\
    "\n"								\
    "This program is free software: you can redistribute it and/or modify\n" \
    "it under the terms of the GNU General Public License as published by\n" \
    "the Free Software Foundation, either version 3 of the License, or\n" \
    "(at your option) any later version.\n"				\
    "\n"								\
    "This program is distributed in the hope that it will be useful,\n" \
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"	\
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"	\
    "GNU General Public License for more details.\n"			\
    "\n"								\
    "You should have received a copy of the GNU General Public License\n" \
    "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";


void set_rlimit() {
#if defined(HAVE_SETRLIMIT) && defined(RLIMIT_CORE)
    rlimit rl;

    rl.rlim_cur = 0;
    rl.rlim_max = 0;
    int retval = setrlimit(RLIMIT_CORE, &rl);
    if (retval != 0) {
	std::cerr << "Failed to suppress the creation of core file." 
		  << std::endl 
		  << "The error message is: " << strerror(errno) 
		  << std::endl
		  << "In case a core file is created, it may contain clear text passwords."
		  << std::endl
		  << std::endl 
		  << "Press <ENTER> to continue"
		  << std::endl;
	char tmp;
	std::cin >> tmp;
    }
#else
    std::cerr << "Cannot suppress the creation of core file."
	      << std::endl 
	      << "In case a core file is created, it may contain clear text passwords."
	      << std::endl
	      << std::endl 
	      << "Press <ENTER> to continue"
	      << std::endl;
    char tmp;
    std::cin >> tmp;
#endif
}

void show_version() {
    std::cout << PACKAGE_STRING << std::endl;
}

void show_copyright() {
    std::cout << COPYRIGHT << std::endl;
}

void show_help(char* prgname) {
    show_version();
    std::cout << std::endl;
    std::cout << prgname
	      << " [-c, -h, -V, -f <filename>]"
	      << std::endl
	      << std::endl;
    std::cout << "-c, --copyright\tshow copyright information"
	      << std::endl
	      << std::endl;
    std::cout << "-f, --file\topen the specified file <filename>"
	      << std::endl
	      << std::endl;
    std::cout << "-h, --help\tshow this help text"
	      << std::endl
	      << std::endl;
    std::cout << "-V, --version\tshow the version of " PACKAGE_NAME
	      << std::endl
	      << std::endl;
    std::cout << PACKAGE_NAME " stores passwords encrypted on disk using the blowfish algorithm."
	      << std::endl;
    std::cout << "The encryption key is computed from the master password using md5, sha1, and"
	      << std::endl;
    std::cout << "ripemd-160 digest algorithms producing a 448 bit (56 bytes) key."
	      << std::endl
	      << std::endl;
}

int main (int argc, char** argv) {
    set_rlimit();

    int c;
    std::string filename;
#ifdef HAVE_GETOPT_LONG
    struct option long_options[] = {
	{"copyright", no_argument, NULL, 'c'},
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'V'},
	{"file", required_argument, NULL, 'f'},
	{NULL,0,NULL,0}
    };
    while ( (c = getopt_long(argc, argv, ":chVf:", long_options, NULL)) != -1) {
#else // HAVE_GETOPT_LONG
    while ( (c = getopt(argc, argv, ":c(copyright)h(help)V(version)f:(file)")) != -1) {
#endif // HAVE_GETOPT_LONG
	switch (c) {
	case 'c':
	    show_copyright();
	    return 0;
	case 'h':
	    show_help(argv[0]);
	    return 0;
	case 'V':
	    show_version();
	    return 0;
	case 'f':
	    filename = optarg;
	    break;
	case ':':
	    std::cerr << "-" << (char)optopt << " without filename"
		      << std::endl;
	    return 1;
	case '?':
	    std::cerr << "unknown argument " << (char)optopt
		      << std::endl;
	    return 1;
	}
    }


    initscr();
    raw();
    noecho();
    refresh();
    curs_set(0);
    keypad (stdscr, TRUE);

    YAPETUI::Colors::initColors();

    MainWindow* mainwin = NULL;
    try {
	mainwin = new MainWindow();
	// filename may be empty
	mainwin->run(filename);
	delete mainwin;
    } catch (std::exception& ex) {
	if (mainwin != NULL)
	    delete mainwin;
	clear();
	endwin();
	std::cerr << ex.what() << std::endl << std::endl;
	return 1;
    }

    clear();
    refresh();
    endwin();

    return 0;
}
