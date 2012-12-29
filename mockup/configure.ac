# $Id$
AC_INIT([libyacurs],[0.1], [rafi@guengel.ch])
AC_CONFIG_SRCDIR([src/curs.cc])
AM_INIT_AUTOMAKE
AC_REVISION([$Revision$])

AC_CONFIG_MACRO_DIR([m4])


AC_LANG([C++])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
AM_PROG_LIBTOOL

AC_CHECK_LIB([curses],[initscr],[],
	[AC_CHECK_LIB([ncurses],[initscr],[],
		[AC_MSG_ERROR([curses/ncurses library required])]
	)]
)

LT_INIT([disable-static])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
