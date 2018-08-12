# $Id$
AC_PREREQ([2.61])
AC_REVISION([$Revision$])
AC_INIT([YAPET],[1.2-dev],[https://bugs.guengel.ch/enter_bug.cgi?product=YAPET])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SUBDIRS([libyacurs])

AM_INIT_AUTOMAKE([check-news dist-xz color-tests])
AM_SILENT_RULES([yes])

# Progs
AC_MSG_NOTICE([Checking required programs])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_MKDIR_P
AM_PROG_AR

LT_INIT
LT_LANG([C++])
AC_LANG([C++])

# "Enables"
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],[enable debug code (Default: no)])],
	[my_debug=$enableval],
	[my_debug=no])
AC_ARG_ENABLE([install-doc],
	[AS_HELP_STRING([--disable-install-doc],[disable installation of documentation (man pages, text and html files; Default: no)])],
	[if test x$enableval = xno ; then
	my_disableinstalldoc=yes
	else
	my_disableinstalldoc=no
	fi],
	[my_disableinstalldoc=no])
AC_ARG_ENABLE([converters],
	[AS_HELP_STRING([--disable-converters],[CSV file import/export utility will not be built (Default: no)])],
	[my_buildconverts=$enableval],
	[my_buildconverts=yes])

# Libs
AX_CHECK_OPENSSL(,[AC_MSG_ERROR([openssl not found])])

# Headers
AC_MSG_NOTICE([Checking C headers])
AC_HEADER_TIME
AC_CHECK_HEADERS([fcntl.h getopt.h inttypes.h libgen.h libintl.h locale.h stdint.h strings.h sys/time.h termios.h])


# Types
AC_MSG_NOTICE([Checking types])
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T

# library functions
AC_MSG_NOTICE([Checking functions])
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([basename isblank isspace lrand48 rand setlocale srand srand48 strcasestr tcgetattr tcsetattr tolower towlower])

AC_CHECK_FUNCS([dup2 getopt memset strchr strdup strerror strstr],,[AC_MSG_ERROR([required function not found])])

LIBS_SAVE="$LIBS"
LDFLAGS_SAVE="$LDFLAGS"
CPPFLAGS_SAVE="$CPPFLAGS"

LIBS="$LIBS $OPENSSL_LIBS"
LDFLAGS="$LDFLAGS $OPENSSL_LDFLAGS"
CPPFLAGS="$CPPFLAGS $OPENSSL_INCLUDES"
AC_MSG_NOTICE([Checking encryption functions])
AC_CHECK_FUNCS([EVP_bf_cbc EVP_CIPHER_CTX_set_key_length EVP_CipherInit_ex EVP_DigestFinal_ex EVP_DigestInit_ex EVP_DigestUpdate EVP_md5 EVP_ripemd160 EVP_sha1],
	[],
	[AC_MSG_ERROR([You are missing a crucial function required for $PACKAGE_NAME])])
AC_CHECK_FUNCS([EVP_CIPHER_CTX_cleanup EVP_CIPHER_CTX_free EVP_CIPHER_CTX_init EVP_CIPHER_CTX_new EVP_MD_CTX_destroy EVP_MD_CTX_create EVP_MD_CTX_free EVP_MD_CTX_new])
AC_MSG_NOTICE([Checking support functions])
AC_CHECK_FUNCS([SSLeay_version])
LIBS="$LIBS_SAVE"
LDFLAGS="$LDFLAGS_SAVE"
CPPFLAGS="$CPPFLAGS_SAVE"

AC_MSG_NOTICE([Checking miscellaneous properties of platform])
AC_CHECK_SIZEOF([int *])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])
AC_C_BIGENDIAN
# Mainly used by the RNG test in tests/rng*
AC_CHECK_FILES([/dev/urandom /dev/random])

AC_MSG_NOTICE([Preparing NLS])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

PKG_CHECK_MODULES([CPPUNIT], [cppunit])

# Some conditionals for the makefileS
AM_CONDITIONAL([USE_NLS], [test x$USE_NLS = xyes])
AM_CONDITIONAL([BUILDCONVERTS], [test x$my_buildconverts = xyes])
AM_CONDITIONAL([DISABLEINSTALLDOC], [test x$my_disableinstalldoc = xyes])
AM_CONDITIONAL([DISABLE_DEBUG], [test x$my_debug = xno])
AC_CONFIG_FILES([Makefile
	po/Makefile.in
	file/Makefile
	crypt/Makefile
	pwgen/Makefile
	ui/Makefile
	glue/Makefile
	yapet/Makefile
	csv2yapet/Makefile
    yapet2csv/Makefile
	tests/Makefile
	tests/file/Makefile
	tests/preload/Makefile
	tests/testpaths.h
	doc/Makefile
	doc/Makefile.doc])
# Inject --disable-install which is used by libyacurs.
ac_configure_args="$ac_configure_args --disable-install"
AC_OUTPUT

echo "******************************************************************"
echo ""
echo "Prefix              : $prefix"
echo "CPPFLAGS            : $CPPFLAGS"
echo "CFLAGS              : $CFLAGS"
echo "CXXFLAGS            : $CXXFLAGS"
echo "LDFLAGS             : $LDFLAGS"
echo ""
echo "Enable debug code   : $my_debug"
echo "Disable install Doc.: $my_disableinstalldoc"
echo "Build csv2yapet     : $my_buildconverts"
echo ""
echo "Use NLS             : $USE_NLS"
echo ""
echo "******************************************************************"
echo ""
echo "Looks good."
echo ""
