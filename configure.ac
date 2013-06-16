# $Id$
AC_PREREQ(2.61)
AC_INIT([YAPET],[1.0],[rafi@guengel.ch])
AM_INIT_AUTOMAKE([silent-rules check-news dist-bzip2 color-tests std-options])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SUBDIRS([libyacurs])


AC_ARG_VAR([AR], [ar command])
AC_ARG_VAR([DOXYGEN],[doxygen command])
AC_ARG_VAR([LYNX], [lynx command])
AC_ARG_VAR([W3M], [w3m command])
AC_ARG_VAR([XSLTPROC], [xsltproc command])

# Progs
AC_MSG_NOTICE([Checking required programs])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_MKDIR_P
AM_PROG_AR
AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen], [no])
AC_CHECK_PROG([XSLTPROC], [xsltproc], [xsltproc], [no])
AC_CHECK_PROG([LYNX], [lynx], [lynx], [no])
AC_CHECK_PROG([W3M], [w3m], [w3m], [no])
LT_INIT([disable-static])
LT_LANG([C++])
AC_LANG([C++])

# Arguments
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
AC_ARG_ENABLE([assert],
	[AS_HELP_STRING([--enable-assert],[enable assert statements for debugging purposes (Default: no)])],
	[my_assert=$enableval],
	[my_assert=no])
AC_ARG_ENABLE([build-doc],
	[AS_HELP_STRING([--enable-build-doc],[enable rebuild of documentation (Requires xsltproc, lynx; Default: no)])],
	[my_builddoc=$enableval],
	[my_builddoc=no])
AC_ARG_ENABLE([install-doc],
	[AS_HELP_STRING([--disable-install-doc],[disable installation of documentation (man pages, text and html files; Default: no)])],
	[if test x$enableval = xno ; then
	my_disableinstalldoc=yes
	else
	my_disableinstalldoc=no
	fi],
	[my_disableinstalldoc=no])
AC_ARG_ENABLE([source-doc],
	[AS_HELP_STRING([--enable-source-doc],[enable build of source code documentention (Requires doxygen; default: no)])],
	[my_buildsourcedoc=$enableval],
	[my_buildsourcedoc=no])
AC_ARG_ENABLE([terminal-title],
	[AS_HELP_STRING([--enable-terminal-title],[set the terminal title (Default: yes)])],
	[my_settitle=$enableval],
	[my_settitle=yes])
AC_ARG_ENABLE([csv2yapet],
	[AS_HELP_STRING([--disable-csv2yapet],[CSV file import utility will not be built (Default: no)])],
	[my_buildcsv2yapet=$enableval],
	[my_buildcsv2yapet=yes])
AC_ARG_ENABLE([pwgen],
	[AS_HELP_STRING([--disable-pwgen],[disable built-in password generator (Default: no)])],
	[my_enablepwgen=$enableval],
	[my_enablepwgen=yes])

if test x$my_settitle = xyes ; then
   AC_DEFINE([HAVE_TERMINALTITLE],[1], [Define if you want YAPET to set the terminal title])
fi

if test x$my_enablepwgen = xyes ; then
   AC_DEFINE([ENABLE_PWGEN],[1], [Define if you want to build support for the password generator])
fi

if ( test x$LYNX = xno && test x$W3M = xno ) || test x$XSLTPROC = xno ; then
   my_builddoc=no
fi

if test x$DOXYGEN = xno ; then
   my_buildsourcedoc=no
fi

# Libs
AC_MSG_NOTICE([Searching for required libraries])
AC_SEARCH_LIBS([EVP_DigestInit_ex],[crypto],[],AC_MSG_ERROR([libcrypto not found]))

# Headers
AC_MSG_NOTICE([Checking C headers])
AC_CHECK_HEADERS([assert.h ctype.h dirent.h errno.h fcntl.h getopt.h limits.h locale.h pwd.h signal.h stdio.h string.h strings.h sys/param.h sys/resource.h termios.h unistd.h setjmp.h])

AC_CHECK_HEADER([openssl/crypto.h], [AC_DEFINE([HAVE_CRYPTO_H],[1],[Set to 1 if you have openssl/crypto.h])])
AC_CHECK_HEADER([openssl/evp.h], [], AC_MSG_ERROR([OpenSSL evp header not found]))

AC_MSG_NOTICE([Checking C++ headers])
AC_CHECK_HEADERS([algorithm cmath exception fstream functional iostream iterator list memory sstream stdexcept string vector],
		       [],
		       [AC_MSG_ERROR([your system is missing a required C++ header file])])

# Types
AC_MSG_NOTICE([Checking types])
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# library functions
AC_MSG_NOTICE([Checking functions])
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([access asctime fchmod fchown fflush floor fprintf fstat getcwd getenv getopt getopt_long getpwuid getuid isblank iscntrl isdigit isprint isspace localtime log10 lrand48 memset pathconf pow rand rename setrlimit sigaction srand srand48 strcasestr strchr strcmp strerror strlen strncasecmp strncmp strncpy strstr strtol tcgetattr tcsetattr time tolower sigsetjmp siglongjmp])

AC_MSG_NOTICE([Checking encryption functions])
AC_CHECK_FUNCS([EVP_bf_cbc EVP_CIPHER_CTX_cleanup EVP_CIPHER_CTX_init EVP_CIPHER_CTX_set_key_length EVP_CipherInit_ex EVP_DigestFinal_ex EVP_DigestInit_ex EVP_DigestUpdate EVP_MD_CTX_cleanup EVP_MD_CTX_init EVP_md5 EVP_ripemd160 EVP_sha1],
	[],
	[AC_MSG_ERROR([You are missing a crucial function required for $PACKAGE_NAME])])
AC_MSG_NOTICE([Checking support functions])
AC_CHECK_FUNCS([SSLeay_version])

AC_MSG_NOTICE([Checking miscellaneous properties of platform])
AC_CHECK_SIZEOF([int *])
AC_C_BIGENDIAN
# Mainly used by the RNG test in tests/rng*
AC_CHECK_FILES([/dev/urandom /dev/random])

AC_MSG_NOTICE([Preparing NLS])
AM_GNU_GETTEXT([external])

# Some conditionals for the makefileS
AM_CONDITIONAL([BUILDDOC], [test x$my_builddoc = xyes])
AM_CONDITIONAL([BUILDSOURCEDOC], [test x$my_buildsourcedoc = xyes])
AM_CONDITIONAL([HAVEDOXYGEN], [test x$DOXYGEN != xno])
AM_CONDITIONAL([HAVEXSLTPROC], [test x$XSLTPROC != xno && ( test x$LYNX != xno || test x$W3M != xno ) ])
AM_CONDITIONAL([HAVEW3M], [ test x$W3M != xno ])
AM_CONDITIONAL([USE_NLS], [test x$USE_NLS = xyes])
AM_CONDITIONAL([USE_INCLUDED_LIBINTL], [test x$USE_INCLUDED_LIBINTL = xyes])
AM_CONDITIONAL([BUILDCSV2YAPET], [test x$my_buildcsv2yapet = xyes])
AM_CONDITIONAL([BUILDPWGEN], [test x$my_enablepwgen = xyes])
AM_CONDITIONAL([DISABLEINSTALLDOC], [test x$my_disableinstalldoc = xyes])
AM_CONDITIONAL([DISABLE_ASSERT], [test x$my_assert = xno])
AC_CONFIG_FILES([Makefile
	po/Makefile.in
	crypt/Makefile
	ui/Makefile
	yapet/Makefile
	yapet/pwgen/Makefile
	csv2yapet/Makefile
	tests/Makefile
	tests/testpaths.h
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
echo "w3m                 : $W3M"
echo "Docbook XSL         : $DOCBOOKXSL"
echo ""
echo "Enable assert       : $my_assert"
echo "Build Documentation : $my_builddoc"
echo "Disable install Doc.: $my_disableinstalldoc"
echo "Build Source Doc.   : $my_buildsourcedoc"
echo "Set Terminal Title  : $my_settitle"
echo "Build csv2yapet     : $my_buildcsv2yapet"
echo "Build with pwgen    : $my_enablepwgen"
echo ""
echo "Use NLS             : $USE_NLS"
echo ""
echo "******************************************************************"
echo ""
echo "Looks good."
echo ""
