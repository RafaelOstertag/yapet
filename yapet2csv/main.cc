// $Id$
//
// Copyright (C) 2014  Rafael Ostertag
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

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#include <unistd.h>

#ifdef HAVE_TERMIOS_H
# include <termios.h>
#endif

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>
#include <libgen.h>

#include "consts.h"
#include "csvexport.h"

#if defined(HAVE_TERMIOS_H) && defined (HAVE_TCSETATTR) && defined (HAVE_TCGETATTR)
#define CAN_DISABLE_ECHO 1
#endif

enum {
    /**
     * Error on command line, i.e. missing parameter.
     */
    ERR_CMDLINE = 1,
    /**
     * The destination file already exists.
     */
    ERR_FILEEXISTS = 2,
    /**
     * A fatal error. Does not indicate convert errors which are handled/logged by \c CSVImport.
     */
    ERR_FATAL = 3,
    /**
     * The max password length
     */
    MAX_PASSWD = 1024,
    /**
     * The max file path length.
     */
    MAX_FILEPATH = 1024
};

const char COPYRIGHT[] = "\nCopyright (C) 2014  Rafael Ostertag\n"  \
                         "\n"                                \
                         "yapet2csv is part of YAPET.\n"                 \
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

/**
 * Disables the echoing of input when using stdin. Uses \c tcgetattr and \c
 * tcsetattr in order to disable the echoing.
 */
void disable_echo() {
#ifdef CAN_DISABLE_ECHO
    struct termios ctios;
    int err = tcgetattr (STDIN_FILENO, &ctios);

    if (err < 0)
        throw std::runtime_error (std::strerror (errno) );

    ctios.c_lflag &= ~ECHO;
    err = tcsetattr (STDIN_FILENO, TCSANOW, &ctios);

    if (err < 0)
        throw std::runtime_error (std::strerror (errno) );

#endif
}

/**
 * Same as \c disable_echo, but reversed.
 */
void enable_echo() {
#ifdef CAN_DISABLE_ECHO
    struct termios ctios;
    int err = tcgetattr (STDIN_FILENO, &ctios);

    if (err < 0)
        throw std::runtime_error (std::strerror (errno) );

    ctios.c_lflag |= ECHO;
    err = tcsetattr (STDIN_FILENO, TCSANOW, &ctios);

    if (err < 0)
        throw std::runtime_error (std::strerror (errno) );

#endif
}

void show_version() {
    std::cout << "yapet2csv is part of ";
    std::cout << PACKAGE_STRING << std::endl;
}

void show_copyright() {
    std::cout << COPYRIGHT << std::endl;
}

void show_help (char* prgname) {
    std::cout << std::endl;
    std::cout << basename(prgname)
              << " [-c] [-h] [-V] [-H] [-p <password>] [-q] [-s <char>] <src> <dst>"
              << std::endl
              << std::endl;
    std::cout << "-c\tshow copyright information"
              << std::endl
              << std::endl;
    std::cout << "-h\tshow this help text"
              << std::endl
              << std::endl;
    std::cout << "-H\tadd header to file"
              << std::endl
              << std::endl;
    std::cout << "-p\tuse <password> to open yapet file"
              << std::endl
              << "\tby the convert."
              << std::endl
              << "\tThe use of this option is discouraged."
              << std::endl
              << std::endl;
    std::cout << "-q\toperate quietly"
              << std::endl
              << std::endl;
    std::cout << "-s\tuse <char> as field separator."
              << std::endl
              << "\tDefault: ,"
              << std::endl
              << std::endl;
    std::cout << "-V\tshow the version of yapet2csv"
              << std::endl
              << std::endl;
    std::cout << "<src>\tthe source YAPET file"
              << std::endl
              << std::endl;
    std::cout << "<dst>\tthe CSV output file"
              << std::endl
              << std::endl;
    std::cout << "yapet2csv converts YAPET files to CSV text files."
              << std::endl
              << std::endl;
}

int main (int argc, char** argv) {
    bool quiet = false;
    bool cmdline_pw = false;
    bool print_header = false;
    char passwd[MAX_PASSWD];
    char separator = ',';
    std::string srcfile;
    std::string dstfile;

    int c;
    extern char *optarg;
    extern int optopt, optind;

    while ( (c = getopt (argc, argv, ":chp:qs:HV") ) != -1) {
        switch (c) {
	case 'c':
	    show_copyright();
	    return 0;
	case 'h':
	    show_help (argv[0]);
	    return 0;
	case 'p':
	    strncpy (passwd, optarg, MAX_PASSWD);
	    passwd[MAX_PASSWD-1] = 0;
	    cmdline_pw = true;
	    break;
	case 'q':
	    quiet = true;
	    break;
	case 's':
	    separator=optarg[0];
	    break;
	case 'V':
	    show_version();
	    return 0;
	case 'H':
	    print_header=true;
	    break;
	case ':':
	    std::cerr << "-" << (char) optopt << " without argument"
		      << std::endl;
	    return ERR_CMDLINE;
	case '?':
	    std::cerr << "unknown argument" << " '" << (char) optopt << "'"
		      << std::endl;
	    return ERR_CMDLINE;
        }
    }

    if (optind < argc && (argc - optind) == 2) {
        srcfile = argv[optind];
        dstfile = argv[++optind];
    } else {
        std::cerr << "Missing argument." << std::endl;

        if (!quiet)
            show_help (argv[0]);

        return ERR_CMDLINE;
    }

    if ( srcfile.find (YAPET::Consts::default_suffix,
                       dstfile.length() -
                       YAPET::Consts::default_suffix.length() )
            == std::string::npos )
        srcfile += YAPET::Consts::default_suffix;

    if (access (dstfile.c_str(), F_OK) == 0) {
        std::cerr << dstfile << " already exists. Aborting." << std::endl;
        return ERR_FILEEXISTS;
    }

    try {
        // We read the password from stdin only if the user did not provide the
        // -p switch.
        if (!cmdline_pw) {
            std::cout << "Please enter the password for " << srcfile << ": ";
            std::cout.flush();
            std::string pw1;
            disable_echo();
            std::getline (std::cin, pw1);
            std::cout << std::endl;
            enable_echo();

            strncpy (passwd, pw1.c_str(), MAX_PASSWD);
	    passwd[MAX_PASSWD-1] = 0;
        }

        CSVExport exp (srcfile, dstfile, separator, !quiet, print_header);
        exp.doexport (passwd);

	if (!quiet) {
	    std::cout << dstfile << " successfully created." << std::endl;
	}
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return ERR_FATAL;
    }

    return 0;
}
