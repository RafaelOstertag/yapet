/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <libgen.h>
#include <unistd.h>

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include "consts.h"
#include "csvimport.h"
#include "intl.h"
#include "openssl.hh"

#if defined(HAVE_TERMIOS_H) && defined(HAVE_TCSETATTR) && \
    defined(HAVE_TCGETATTR)
#define CAN_DISABLE_ECHO 1
#endif

enum {
    /**
     * Error on command line, i.e. missing parameter.
     */
    ERR_CMDLINE = 1,
    /**
     * The passwords supplied by the user do not match.
     */
    ERR_PASSWDMISMATCH = 2,
    /**
     * The destination file already exists.
     */
    ERR_FILEEXISTS = 3,
    /**
     * A fatal error. Does not indicate convert errors which are handled/logged
     * by \c CSVImport.
     */
    ERR_FATAL = 4,
    /**
     * The max password length
     */
    MAX_PASSWD = 1024,
    /**
     * The max file path length.
     */
    MAX_FILEPATH = 1024
};

const char COPYRIGHT[] =
    "\nCopyright (C) 2009-2011, 2013, 2014  Rafael Ostertag\n"
    "\n"
    "csv2yapet is part of YAPET.\n"
    "\n"
    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";

/**
 * Disables the echoing of input when using stdin. Uses \c tcgetattr and \c
 * tcsetattr in order to disable the echoing.
 */
void disable_echo() {
#ifdef CAN_DISABLE_ECHO
    struct termios ctios;
    int err = tcgetattr(STDIN_FILENO, &ctios);

    if (err < 0) throw std::runtime_error(std::strerror(errno));

    ctios.c_lflag &= ~ECHO;
    err = tcsetattr(STDIN_FILENO, TCSANOW, &ctios);

    if (err < 0) throw std::runtime_error(std::strerror(errno));

#endif
}

/**
 * Same as \c disable_echo, but reversed.
 */
void enable_echo() {
#ifdef CAN_DISABLE_ECHO
    struct termios ctios;
    int err = tcgetattr(STDIN_FILENO, &ctios);

    if (err < 0) throw std::runtime_error(std::strerror(errno));

    ctios.c_lflag |= ECHO;
    err = tcsetattr(STDIN_FILENO, TCSANOW, &ctios);

    if (err < 0) throw std::runtime_error(std::strerror(errno));

#endif
}

void show_version() {
    std::cout << _("csv2yapet is part of ");
    std::cout << PACKAGE_STRING << std::endl;
}

void show_copyright() { std::cout << COPYRIGHT << std::endl; }

void show_help(char* prgname) {
    std::cout << std::endl;
    std::cout << basename(prgname)
              << " [-h] [-p <password>] [-q] [-s <char>] [-V] <src> <dst>"
              << std::endl
              << std::endl;
    std::cout << "-c\t" << _("show copyright information") << std::endl
              << std::endl;
    std::cout << "-h\t" << _("show this help text") << std::endl << std::endl;
    std::cout << "-p\t"
              << _("use <password> as the master password for the YAPET file.")
              << std::endl
              << "\t" << _("The use of this option is discouraged.")
              << std::endl
              << std::endl;
    std::cout << "-q\t" << _("operate quietly") << std::endl << std::endl;
    std::cout << "-s\t:" << _("use <char> as field separator.") << std::endl
              << "\t" << _("Default: ,") << std::endl
              << std::endl;
    std::cout << "-V\t" << _("show the version of csv2yapet") << std::endl
              << std::endl;
    std::cout << "<src>\t" << _("the source csv file") << std::endl
              << std::endl;
    std::cout << "<dst>\t" << _("the output file") << std::endl << std::endl;
    std::cout << _("csv2yapet converts CSV text files to YAPET files.")
              << std::endl
              << std::endl;
}

int main(int argc, char** argv) {
    bool quiet = false;
    bool cmdline_pw = false;
    char passwd[MAX_PASSWD];
    char separator = ',';
    std::string srcfile;
    std::string dstfile;

    int c;
    extern char* optarg;
    extern int optopt, optind;

    while ((c = getopt(argc, argv, ":chp:qs:V")) != -1) {
        switch (c) {
            case 'c':
                show_copyright();
                return 0;
            case 'h':
                show_help(argv[0]);
                return 0;
            case 'p':
                strncpy(passwd, optarg, MAX_PASSWD - 1);
                passwd[MAX_PASSWD - 1] = 0;
                cmdline_pw = true;
                break;
            case 'q':
                quiet = true;
                break;
            case 's':
                separator = optarg[0];
                break;
            case 'V':
                show_version();
                return 0;
            case ':':
                std::cerr << "-" << (char)optopt << _(" without argument")
                          << std::endl;
                return ERR_CMDLINE;
            case '?':
                std::cerr << _("unknown argument") << " '" << (char)optopt
                          << "'" << std::endl;
                return ERR_CMDLINE;
        }
    }

    if (optind < argc && (argc - optind) == 2) {
        srcfile = argv[optind];
        dstfile = argv[++optind];
    } else {
        std::cerr << _("Missing argument.") << std::endl;

        if (!quiet) show_help(argv[0]);

        return ERR_CMDLINE;
    }

    if (dstfile.find(
            YAPET::Consts::DEFAULT_FILE_SUFFIX,
            dstfile.length() - YAPET::Consts::DEFAULT_FILE_SUFFIX.length()) ==
        std::string::npos)
        dstfile += YAPET::Consts::DEFAULT_FILE_SUFFIX;

    if (access(dstfile.c_str(), F_OK) == 0) {
        std::cerr << dstfile << _(" already exists. Aborting.") << std::endl;
        return ERR_FILEEXISTS;
    }

    yapet::OpenSSL::init();

    try {
        // We read the password from stdin only if the user did not provide the
        // -p switch.
        if (!cmdline_pw) {
            std::cout << _("Please enter the password for ") << dstfile << ": ";
            std::cout.flush();
            std::string pw1;
            disable_echo();
            std::getline(std::cin, pw1);
            std::cout << std::endl;
            enable_echo();
            std::cout << _("Please re-type the password: ");
            std::cout.flush();
            std::string pw2;
            disable_echo();
            std::getline(std::cin, pw2);
            std::cout << std::endl;
            enable_echo();
            std::cout << std::endl;

            if (pw1 != pw2) {
                std::cerr << _("Passwords do not match.") << std::endl;
                return ERR_PASSWDMISMATCH;
            }

            strncpy(passwd, pw1.c_str(), MAX_PASSWD - 1);
            passwd[MAX_PASSWD - 1] = '\0';
        }

        CSVImport imp(srcfile, dstfile, separator, !quiet);
        imp.import(passwd);

        if (imp.hadErrors() && !quiet) {
            std::cout << _("Had ") << imp.numErrors() << _(" errors:")
                      << std::endl;
            imp.printLog();
            std::cout << std::endl;
            std::cout << dstfile << _(" created with errors.") << std::endl;
        } else {
            if (!quiet) {
                std::cout << dstfile << _(" successfully created.")
                          << std::endl;
            }
        }
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return ERR_FATAL;
    }

    return 0;
}
