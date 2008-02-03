# $Id$
AC_PREREQ(2.61)
AC_INIT([GPSafe],[0.1],[rafi@guengel.ch])
AM_INIT_AUTOMAKE
AC_REVISION([$Rev$])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Progs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_RANLIB

AC_LANG([C++])

# Arguments
AC_ARG_WITH([curses-include],
	[AS_HELP_STRING([--with-curses-include],[directory for (n)curses header files])],
	[CPPFLAGS="-I$withval $CPPFLAGS"],
	[])
AC_ARG_WITH([curses-lib],
	[AS_HELP_STRING([--with-curses-lib],[directory for (n)curses libraries])],
	[LDFLAGS="-L$withval $LDFLAGS"],
	[])
AC_ARG_WITH([openssl-include],
	[AS_HELP_STRING([--with-openssl-include],[directory for openssl header files])],
	[CPPFLAGS="-I$withval $CPPFLAGS"],
	[])
AC_ARG_WITH([openssl-lib],
	[AS_HELP_STRING([--with-openssl-lib],[directory for openssl libraries])],
	[LDFLAGS="-L$withval $LDFLAGS"],
	[])
	

# Libs
AC_SEARCH_LIBS([initscr],[ncurses curses],[],AC_MSG_ERROR([curses not found]))
AC_SEARCH_LIBS([EVP_DigestInit_ex],[crypto],[],AC_MSG_ERROR([libcrypto not found]))

# Headers
AC_INCLUDES_DEFAULT
AC_CHECK_HEADERS([fcntl.h string.h unistd.h])
AC_HEADER_TIME
AC_CHECK_HEADER([ncurses.h],
	[AC_DEFINE([HAVE_NCURSES_H],[1],[Set to 1 if you have ncurses.h])],
	[AC_CHECK_HEADER([curses.h],
		[AC_DEFINE([HAVE_CURSES_H],[1],[Set to 1 if you have curses.h])],
		[AC_MSG_ERROR([curses header not found])])]
)
AC_CHECK_HEADER([openssl/evp.h], [], AC_MSG_ERROR([OpenSSL evp header not found]))

# Types
AC_TYPE_INT32_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# library functions
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([memset strerror getcwd pathconf])
AC_CHECK_FUNCS([wresize mvwchgat],[],[])

# Some custom tests
AC_MSG_CHECKING([whether waddstr() uses const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); waddstr(stdscr, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no]) AC_DEFINE([WADDSTR_USE_CHAR],[1],[Set to 1 if waddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddstr() uses const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddstr(stdscr, 2, 1, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no]) AC_DEFINE([MVWADDSTR_USE_CHAR],[1],[Set to 1 if mvwaddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddnstr() uses const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddnstr(stdscr, 2, 1, teststr, 4); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no]) AC_DEFINE([MVWADDNSTR_USE_CHAR],[1],[Set to 1 if mvwaddnstr() doesn't use const char*])])

AC_CONFIG_FILES([Makefile
	gpsafe/Makefile
	ui/Makefile
	crypt/Makefile
	tests/Makefile])
AC_OUTPUT
