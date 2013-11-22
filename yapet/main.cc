// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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
# include "config.h"
#endif

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

#include <unistd.h>
#include <sys/resource.h>

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#include <cstring>
#include <cerrno>
#include <iostream>
#include <string>

#include <yacurs.h>

#include "mainwindow.h"
#include "globals.h"
#include "consts.h"
#include "yapetlockscreen.h"
#include "yapetunlockdialog.h"

/**
 * @file
 *
 * @brief The file holding the \c main() function.
 *
 * Holds the \c main() function. It does also process command line arguments
 * and initialize curses.
 */

const char COPYRIGHT[] = "YAPET -- Yet Another Password Encryption Tool\n" \
                         "Copyright (C) 2008-2013  Rafael Ostertag\n"               \
                         "\n"                                \
                         "This program is free software: you can redistribute it and/or modify\n" \
                         "it under the terms of the GNU General Public License as published by\n" \
                         "the Free Software Foundation, either version 3 of the License, or\n" \
                         "(at your option) any later version.\n"             \
                         "\n"                                \
                         "This program is distributed in the hope that it will be useful,\n" \
                         "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"  \
                         "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"   \
                         "GNU General Public License for more details.\n"            \
                         "\n"                                \
                         "You should have received a copy of the GNU General Public License\n" \
                         "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";

void
set_rlimit() {
#if defined(RLIMIT_CORE) && !defined(DEBUG)
    rlimit rl;
    rl.rlim_cur = 0;
    rl.rlim_max = 0;
    int retval = setrlimit(RLIMIT_CORE, &rl);

    if (retval != 0) {
        std::cerr << _("Failed to suppress the creation of core file.")
                  << std::endl
                  << _("The error message is: ") << strerror(errno)
                  << std::endl
                  << _(
            "In case a core file is created, it may contain clear text passwords.")
                  << std::endl
                  << std::endl
                  << _("Press <ENTER> to continue")
                  << std::endl;
        std::cin.ignore(1, '\n');
    }

#else
# if !defined(DEBUG)
    std::cerr << _("Cannot suppress the creation of core file.")
              << std::endl
              << _(
        "In case a core file is created, it may contain clear text passwords.")
              << std::endl
              << std::endl
              << _("Press <ENTER> to continue")
              << std::endl;
    std::cin.ignore(1, '\n');
# endif
#endif
}

void
show_version() {
    std::cout << PACKAGE_STRING << std::endl;
}

void
show_copyright() {
    std::cout << COPYRIGHT << std::endl;
}

void
show_help(char* prgname) {
    show_version();
    std::cout << std::endl;
    std::cout << prgname
              <<
        " [-chV] [-i | -r <rcfile>] [-s | -S] [-t <sec>] [<filename>]"
              << std::endl
              << std::endl;
    std::cout << "-c, --copyright\t\t" << _("show copyright information")
              << std::endl
              << std::endl;
    std::cout << "-h, --help\t\t" << _("show this help text")
              << std::endl
              << std::endl;
    std::cout << "-i, --ignore-rc\t\t" << _(
        "do not read the configuration file.")
              << std::endl
              << std::endl;
    std::cout << "-r, --rc-file\t\t" << _("read the configuration file specified by <rcfile>.\n" \
                                          "\t\t\tIf this option is not provided, it defaults to\n" \
                                          "\t\t\t$HOME/.yapet unless -i is specified.")
              << std::endl
              << std::endl;
    std::cout << "-s, --no-file-security\t" << _("disable check of owner and file permissions.\n" \
                                                 "\t\t\tWhen creating new files, the file mode is set\n" \
                                                 "\t\t\tto 0644.")
              << std::endl
              << std::endl;
    std::cout << "-S, --file-security\t" << _("enable check of owner and file permissions.\n" \
                                              "\t\t\tWhen creating new files, the file mode is set\n" \
                                              "\t\t\tto 0600.")
              << std::endl
              << std::endl;
    std::cout << "-t, --timeout\t\t" << _("the time-out in seconds until the screen is locked.\n" \
                                          "\t\t\tDefault: 600 sec.")
              << std::endl
              << std::endl;
    std::cout << "-V, --version\t\t" << _("show the version of ") <<
        PACKAGE_NAME
              << std::endl
              << std::endl;
    std::cout << "<filename>\t\t" << _("open the specified file <filename>")
              << std::endl
              << std::endl;
    char buff[512];
    snprintf(buff, 512,
             _(
                 "%s stores passwords encrypted on disk using the blowfish encryption\n" \
                 "algorithm. Point your browser to http://www.guengel.ch/myapps/yapet/\n"
                 "for more information about %s."), PACKAGE_NAME,
             PACKAGE_NAME);
    std::cout << buff
              << std::endl
              << std::endl;
}

int
main(int argc, char** argv) {
    unsigned int timeout = 0;

    set_rlimit();

#ifdef ENABLE_NLS
# if !defined(HAVE_SETLOCALE)
#  error "NLS support requested, but no setlocale() found."
# endif
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);
#endif

    // If empty, default is taken
    std::string cfgfilepath;
    int c;

#ifdef HAVE_GETOPT_LONG
    struct option long_options[] = {
        { (char*)"copyright", no_argument, NULL, 'c'},
        { (char*)"help", no_argument, NULL, 'h'},
        { (char*)"ignore-rc", no_argument, NULL, 'i'},
        { (char*)"rc-file", required_argument, NULL, 'r'},
        { (char*)"no-file-security", no_argument, NULL, 's'},
        { (char*)"file-security", no_argument, NULL, 'S'},
        { (char*)"timeout", required_argument, NULL, 't'},
        { (char*)"version", no_argument, NULL, 'V'},
        {NULL, 0, NULL, 0}
    };

    while ( (c =
                 getopt_long(argc, argv, ":chir:sSt:V", long_options,
                             NULL) ) != -1) {
#else // HAVE_GETOPT_LONG
    extern char* optarg;
    extern int optopt, optind;

    while ( (c =
                 getopt(argc, argv,
                        ":c(copyright)h(help)i(ignore-rc)r:(rc-file)s(no-file-security)S(file-security)t:(timeout)V(version)") )
            != -1) {
#endif // HAVE_GETOPT_LONG

        switch (c) {
        case 'c':
            show_copyright();
            return 0;

        case 'h':
            show_help(argv[0]);
            return 0;

        case 'i':
#warning "FIX"
	    // YAPET::Globals::config.ignorerc.set(true);
            // YAPET::Globals::config.ignorerc.lock();
            break;

        case 'r':
            cfgfilepath = optarg;
            break;

        case 'V':
            show_version();
            return 0;

        case 's':
            YAPET::Globals::config.filesecurity.set(false);
            YAPET::Globals::config.filesecurity.lock();
            break;

        case 'S':
            YAPET::Globals::config.filesecurity.set(true);
            YAPET::Globals::config.filesecurity.lock();
            break;

        case 't':
            sscanf(optarg, "%u", &timeout);
            YAPET::Globals::config.timeout.set(timeout);
            YAPET::Globals::config.timeout.lock();
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
        YAPET::Globals::config.petfile.set(argv[optind]);
        YAPET::Globals::config.petfile.lock();
    }

    // try{}, may be the file does not exist
    try {
	YAPET::CONFIG::ConfigFile cfgfile(YAPET::Globals::config,cfgfilepath);
	cfgfile.parse();
    } catch(std::runtime_error&) {
	// intentionally empty
    }

    // Unlock all configuration values, so that they can be changed
    // again.
    YAPET::Globals::config.unlock();

     YapetUnlockDialog* yunlockdia=0;
    try {
        YACURS::Curses::init();

#ifndef DEBUG
        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   PACKAGE_STRING) );
#else
        YACURS::Curses::title(new YACURS::TitleBar(YACURS::TitleBar::
                                                   POS_TOP,
                                                   PACKAGE_STRING " (DEBUG Build)") );
#endif
	YACURS::Curses::title()->alignment(YACURS::LineObject::CENTER);

	YACURS::Curses::statusbar(new YACURS::StatusBar());
	YACURS::Curses::statusbar()->push(_("Press 'H' for help"));

	YACURS::Curses::mainwindow(new MainWindow());

	if (YAPET::Globals::config.timeout>0) {
	    yunlockdia = new YapetUnlockDialog(*YACURS::Curses::mainwindow());
	    YACURS::EventQueue::lock_screen(new YapetLockScreen(*YACURS::Curses::mainwindow(),
								yunlockdia,  YAPET::Globals::config.timeout, YAPET::Globals::config.pw_input_timeout));
	}
	    

	YACURS::Curses::run();

        delete YACURS::Curses::mainwindow();
	delete YACURS::Curses::title();
	delete YACURS::Curses::statusbar();

	if (YACURS::EventQueue::lock_screen())
	    delete YACURS::EventQueue::lock_screen();

	if (yunlockdia)
	    delete yunlockdia;
    } catch (std::exception& ex) {
    	if (YACURS::Curses::mainwindow())
    	    delete YACURS::Curses::mainwindow();

    	if (YACURS::Curses::title())
    	    delete YACURS::Curses::title();

    	if (YACURS::Curses::statusbar())
    	    delete YACURS::Curses::statusbar();

    	if (YACURS::EventQueue::lock_screen())
    	    delete YACURS::EventQueue::lock_screen();

    	if (yunlockdia)
    	    delete yunlockdia;

        try {
            YACURS::Curses::end();
        } catch (...) { /* Empty */
        }
        std::cerr << ex.what() << std::endl << std::endl;
        return 1;
    }

    YACURS::Curses::end();

    return 0;
}
