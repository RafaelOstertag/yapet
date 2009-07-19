// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
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

#ifdef HAVE_CRYPTO_H
# include <openssl/crypto.h>
#endif

#include "../intl.h"
#include "fileopen.h"
#include "mainwindow.h"
#include "cfgfile.h"
#include "consts.h"

/**
 * @file
 *
 * @brief The file holding the \c main() function.
 *
 * Holds the \c main() function. It does also process command line arguments
 * and initialize curses.
 */

const char COPYRIGHT[] = "YAPET -- Yet Another Password Encryption Tool\n" \
    "Copyright (C) 2008, 2009  Rafael Ostertag\n"				\
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
	std::cerr << _("Failed to suppress the creation of core file.")
		  << std::endl
		  << _("The error message is: ") << strerror(errno)
		  << std::endl
		  << _("In case a core file is created, it may contain clear text passwords.")
		  << std::endl
		  << std::endl
		  << _("Press <ENTER> to continue")
		  << std::endl;
	char tmp;
	std::cin >> tmp;
    }
#else
    std::cerr << _("Cannot suppress the creation of core file.")
	      << std::endl
	      << _("In case a core file is created, it may contain clear text passwords.")
	      << std::endl
	      << std::endl
	      << _("Press <ENTER> to continue")
	      << std::endl;
    char tmp;
    std::cin >> tmp;
#endif
}

void show_version() {
    std::cout << PACKAGE_STRING << std::endl;

#ifdef HAVE_SSLEAY_VERSION
    std::cout << "SSL Version: " << SSLeay_version(SSLEAY_VERSION) << std::endl;
#endif

#ifdef NCURSES_VERSION
    std::cout << "Curses Implementation: " << "ncurses (" << NCURSES_VERSION << ")" << std::endl;
#else	// NCURSES_VERSION
#ifdef _XOPEN_CURSES
    std::cout << "Curses Implementation: " << "XOpen Curses" << std::endl;
#else	// _XOPEN_CURSES
    std::cout << "Curses Implementation: " << "System Curses" << std::endl;
#endif	// _XOPEN_CURSES
#endif	// NCURSES_VERSION

#if defined(HAVE_TERMINALTITLE) && defined(HAVE_TERMNAME)
    std::cout << "Compiled with support for terminal title" << std::endl;
#else
    std::cout << "Compiled without support for terminal title" << std::endl;
#endif

#if !defined(HAVE_FSTAT) || !defined(HAVE_GETUID) || !defined(HAVE_FCHMOD) || !defined(HAVE_FCHOWN)
    std::cout << "Support for file security not available" << std::endl;
#endif
}

void show_copyright() {
    std::cout << COPYRIGHT << std::endl;
}

void show_help(char* prgname) {
    show_version();
    std::cout << std::endl;
    std::cout << prgname
	      << " [[-c] | [-h] | [-V]] [[-s] | [-S]] [-t <sec>] [<filename>]"
	      << std::endl
	      << std::endl;
    std::cout << "-c, --copyright\t\tshow copyright information"
	      << std::endl
	      << std::endl;
    std::cout << "-h, --help\t\tshow this help text"
	      << std::endl
	      << std::endl;
    std::cout << "-s, --no-file-security\tdisable check of owner and file permissions."
	      << std::endl
	      << "\t\t\tWhen creating new files, the file mode is set"
	      << std::endl
	      << "\t\t\tto 0644."
	      << std::endl
	      << std::endl;
    std::cout << "-S, --file-security\tenable check of owner and file permissions."
	      << std::endl
	      << "\t\t\tWhen creating new files, the file mode is set"
	      << std::endl
	      << "\t\t\tto 0600."
	      << std::endl
	      << std::endl;
    std::cout << "-t, --timeout\t\tthe time-out in seconds until the screen is locked."
	      << std::endl
	      << "\t\t\tDefault: 600 sec."
	      << std::endl
	      << std::endl;
    std::cout << "-V, --version\t\tshow the version of " PACKAGE_NAME
	      << std::endl
	      << std::endl;
    std::cout << "<filename>\t\topen the specified file <filename>"
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

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    int c;
    std::string filename;
    bool filesecurity = true;
    int timeout = 600;

    ConfigFile* cfg = new ConfigFile();
    filename = cfg->getFileToLoad();
    filesecurity = cfg->getUseFileSecurity();
    timeout = cfg->getLockTimeout();
    delete cfg;

#ifdef HAVE_GETOPT_LONG
    struct option long_options[] = {
	{"copyright", no_argument, NULL, 'c'},
	{"help", no_argument, NULL, 'h'},
	{"no-file-security", no_argument, NULL, 's'},
	{"file-security", no_argument, NULL, 'S'},
	{"timeout", required_argument, NULL, 't'},
	{"version", no_argument, NULL, 'V'},
	{NULL,0,NULL,0}
    };
    while ( (c = getopt_long(argc, argv, ":chsSt:V", long_options, NULL)) != -1) {
#else // HAVE_GETOPT_LONG
	extern char *optarg;
	extern int optopt, optind;
    while ( (c = getopt(argc, argv, ":c(copyright)h(help)s(no-file-security)S(file-security)t:(timeout)V(version)")) != -1) {
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
	case 's':
	    filesecurity = false;
	    break;
	case 'S':
	    filesecurity = true;
	    break;
	case 't':
	    sscanf(optarg, "%u", &timeout);
	    break;
	case ':':
	    std::cerr << "-" << (char)optopt << _(" without argument")
		      << std::endl;
	    return 1;
	case '?':
	    std::cerr << _("unknown argument") << " '" << (char)optopt << "'"
		      << std::endl;
	    return 1;
	}
    }

    if (optind < argc) {
	filename = argv[optind];

	if (!endswith(filename, Consts::getDefaultSuffix()))
	    filename+=Consts::getDefaultSuffix();
    }

    YAPETUI::BaseWindow::initCurses();

    MainWindow* mainwin = NULL;
    try {
	mainwin = new MainWindow(timeout, filesecurity);
	// filename may be empty
	mainwin->run(filename);
	delete mainwin;
    } catch (std::exception& ex) {
	if (mainwin != NULL)
	    delete mainwin;
	YAPETUI::BaseWindow::endCurses();
	std::cerr << ex.what() << std::endl << std::endl;
	return 1;
    }

    YAPETUI::BaseWindow::endCurses();

    return 0;
}
