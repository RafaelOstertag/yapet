# $Id$
AC_PREREQ(2.61)
AC_INIT([YAPET],[0.4],[rafi@guengel.ch])
AM_INIT_AUTOMAKE([yapet],[0.4])
AC_REVISION([$Revision$])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])


# Progs
AC_MSG_NOTICE([Checking required programs])
AC_PROG_CC([cc gcc])
AC_PROG_CXX([CC c++ g++])
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_RANLIB
AC_PROG_MKDIR_P
AC_PROG_LEX
AC_PROG_YACC
AC_ARG_VAR([DOXYGEN],[path to doxygen])
AC_ARG_VAR([XSLTPROC], [path to xsltproc])
AC_ARG_VAR([LYNX], [path to lynx])
AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen], [no])
AC_CHECK_PROG([XSLTPROC], [xsltproc], [xsltproc], [no])
AC_CHECK_PROG([LYNX], [lynx], [lynx], [no])
AC_USE_SYSTEM_EXTENSIONS
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
AC_SUBST([DOCBOOKXSL],[http://docbook.sourceforge.net/release/xsl/current])
AC_ARG_WITH([docbook-xsl],
	[AS_HELP_STRING([--with-docbook-xsl],[directory where to find the Docbook XSL])],
	[AC_SUBST([DOCBOOKXSL],[$withval])])

# "Enables"
AC_ARG_ENABLE([build-doc],
	[AS_HELP_STRING([--enable-build-doc],[build docs])],
	[my_builddoc=yes],
	[my_builddoc=no])
AC_ARG_ENABLE([terminal-title],
	[AS_HELP_STRING([--enable-terminal-title],[YAPET tries to set the terminal title])],
	[AC_DEFINE([HAVE_TERMINALTITLE],[1], [Define if you want YAPET to set the terminal title])
	my_settitle=yes],
	[my_settitle=no])

# Libs
AC_MSG_NOTICE([Searching for required libraries])
AC_SEARCH_LIBS([initscr],[ncurses curses],[],AC_MSG_ERROR([curses not found]))
AC_SEARCH_LIBS([EVP_DigestInit_ex],[crypto],[],AC_MSG_ERROR([libcrypto not found]))

# Headers
AC_MSG_NOTICE([Checking C headers])
AC_CHECK_HEADERS([stdio.h fcntl.h strings.h string.h unistd.h limits.h sys/param.h sys/resource.h errno.h getopt.h signal.h locale.h])

AC_CHECK_HEADER([ncurses.h],
	[AC_DEFINE([HAVE_NCURSES_H],[1],[Set to 1 if you have ncurses.h]) my_usencurses=yes],
	[AC_CHECK_HEADER([curses.h],
		[AC_DEFINE([HAVE_CURSES_H],[1],[Set to 1 if you have curses.h]) my_usencurses=no],
		[AC_MSG_ERROR([curses header not found])])]
)
AC_CHECK_HEADER([openssl/crypto.h], [AC_DEFINE([HAVE_CRYPTO_H],[1],[Set to 1 if you have openssl/crypto.h])])
AC_CHECK_HEADER([openssl/evp.h], [], AC_MSG_ERROR([OpenSSL evp header not found]))

AC_MSG_NOTICE([Checking C++ headers])
AC_CHECK_HEADERS([list exception stdexcept string iostream algorithm iterator memory functional],
		       [],
		       [AC_MSG_ERROR([your system is missing a required C++ header file])])

# Types
AC_MSG_NOTICE([Checking types])
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_INT32_T
AC_TYPE_SSIZE_T
AC_TYPE_SIZE_T
AC_TYPE_OFF_T

# library functions
AC_MSG_NOTICE([Checking functions])
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([memset strerror strcmp strncmp strncasecmp strcasestr strstr getcwd pathconf setrlimit getopt getopt_long rename sigaction asctime localtime fflush fprintf fstat getuid fchmod fchown])

AC_MSG_NOTICE([Checking (n)curses functions])
AC_CHECK_FUNCS([wresize mvwchgat termname])

# Some custom tests
AC_MSG_CHECKING([whether waddstr() use const char*])
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
	[AC_MSG_RESULT([no])
	AC_DEFINE([WADDSTR_USE_CHAR],[1],[Set to 1 if waddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddstr() use const char*])
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
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDSTR_USE_CHAR],[1],[Set to 1 if mvwaddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddnstr() use const char*])
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
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDNSTR_USE_CHAR],[1],[Set to 1 if mvwaddnstr() doesn't use const char*])])

AC_MSG_NOTICE([Checking encryption functions])
AC_CHECK_FUNCS([EVP_bf_cbc EVP_sha1 EVP_md5 EVP_ripemd160],
	[],
	[AC_MSG_ERROR([You are missing a crucial function required for $PACKAGE_NAME])])
AC_MSG_NOTICE([Checking support functions])
AC_CHECK_FUNCS([SSLeay_version])


AC_C_BIGENDIAN


AC_MSG_NOTICE([Intl support])
AM_GNU_GETTEXT

# Some conditionals for the makefileS
AM_CONDITIONAL([BUILDDOC], [test x$my_builddoc = xyes])
AM_CONDITIONAL([HAVEDOXYGEN], [test x$DOXYGEN != xno])
AM_CONDITIONAL([HAVEXSLTPROC], [test x$XSLTPROC != xno -a x$LYNX != xno])
AM_CONDITIONAL([USE_INCLUDED_LIBINTL], [test x$USE_INCLUDED_LIBINTL = xyes])
AC_CONFIG_FILES([Makefile
	intl/Makefile
	po/Makefile.in
	yapet/Makefile
	ui/Makefile
	crypt/Makefile
	tests/Makefile
	doc/Makefile])
AC_OUTPUT

echo "******************************************************************"
echo ""
echo "Prefix              : $prefix"
echo "CPPFLAGS            : $CPPFLAGS"
echo "CFLAGS              : $CFLAGS"
echo "CXXFLAGS            : $CXXFLAGS"
echo "LDFLAGS             : $LDFLAGS"
echo "Use ncurses.h       : $my_usencurses"
echo ""
echo "Doxygen             : $DOXYGEN"
echo "xsltproc            : $XSLTPROC"
echo "Lynx                : $LYNX"
echo "Docbook XSL         : $DOCBOOKXSL"
echo ""
echo "Build Documentation : $my_builddoc"
echo "Set Terminal Title  : $my_settitle"
echo ""
echo "Use included libintl: $USE_INCLUDED_LIBINTL"
echo ""
echo "******************************************************************"
echo ""
echo "Looks good."
echo ""
