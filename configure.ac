# $Id$
AC_PREREQ([2.61])
AC_REVISION([$Revision$])
AC_INIT([YAPET],[2.0],[https://bugs.guengel.ch/enter_bug.cgi?product=YAPET])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SUBDIRS([libyacurs])

AM_INIT_AUTOMAKE([check-news dist-xz dist-bzip2 color-tests])
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

AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

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

# Libs
AX_CHECK_OPENSSL(,[AC_MSG_ERROR([openssl not found])])

# Headers
AC_MSG_NOTICE([Checking C headers])
AC_CHECK_HEADERS([fcntl.h getopt.h libgen.h libintl.h locale.h strings.h termios.h])

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
AC_CHECK_FUNCS([basename isblank isspace setlocale strcasestr tcgetattr tcsetattr tolower towlower])

AC_CHECK_FUNCS([getopt strchr strdup strerror strstr],,[AC_MSG_ERROR([required function not found])])

LIBS_SAVE="$LIBS"
LDFLAGS_SAVE="$LDFLAGS"
CPPFLAGS_SAVE="$CPPFLAGS"

LIBS="$LIBS $OPENSSL_LIBS"
LDFLAGS="$LDFLAGS $OPENSSL_LDFLAGS"
CPPFLAGS="$CPPFLAGS $OPENSSL_INCLUDES"
AC_MSG_NOTICE([Checking encryption functions])
AC_CHECK_FUNCS([EVP_bf_cbc EVP_CIPHER_CTX_set_key_length EVP_CipherInit_ex EVP_DigestFinal_ex EVP_DigestInit_ex EVP_DigestUpdate EVP_md5 EVP_ripemd160 EVP_sha1 EVP_sha256 EVP_aes_256_cbc RAND_bytes],
	[],
	[AC_MSG_ERROR([You are missing a crucial function required for $PACKAGE_NAME])])
AC_CHECK_FUNCS([EVP_CIPHER_CTX_cleanup EVP_CIPHER_CTX_free EVP_CIPHER_CTX_init EVP_CIPHER_CTX_new EVP_MD_CTX_destroy EVP_MD_CTX_create EVP_MD_CTX_free EVP_MD_CTX_new])
AC_MSG_NOTICE([Checking support functions])
AC_CHECK_FUNCS([SSLeay_version OpenSSL_version])
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
PKG_CHECK_MODULES([ARGON2], [libargon2])

# Some conditionals for the makefileS
AM_CONDITIONAL([USE_NLS], [test x$USE_NLS = xyes])
AM_CONDITIONAL([DISABLEINSTALLDOC], [test x$my_disableinstalldoc = xyes])
AM_CONDITIONAL([DISABLE_DEBUG], [test x$my_debug = xno])
AC_CONFIG_FILES([
	doc/Makefile
	doc/Makefile.doc
	Makefile
	po/Makefile.in
	src/converters/csv/Makefile
	src/converters/csv2yapet/Makefile
	src/converters/Makefile
	src/converters/yapet2csv/Makefile
	src/libs/cfg/Makefile
	src/libs/consts/Makefile
	src/libs/crypt/Makefile
	src/libs/exceptions/Makefile
	src/libs/file/Makefile
	src/libs/globals/Makefile
	src/libs/glue/Makefile
	src/libs/interfaces/Makefile
	src/libs/Makefile
	src/libs/metadata/Makefile
	src/libs/passwordrecord/Makefile
	src/libs/pwgen/Makefile
	src/libs/ui/Makefile
	src/libs/utils/Makefile
	src/Makefile
	src/yapet/Makefile
	tests/cfg/Makefile
	tests/cfg/testpaths.h
	tests/converters/csv/Makefile
	tests/converters/import/Makefile
	tests/converters/import/testpaths.h
	tests/converters/Makefile
	tests/crypt/Makefile
	tests/crypt/testpaths.h
	tests/file/Makefile
	tests/file/testpaths.h
	tests/Makefile
	tests/metadata/Makefile
	tests/passwordrecord/Makefile
	tests/preload/Makefile
	tests/pwgen/Makefile
	tests/utils/Makefile
	])
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
echo ""
echo "Use NLS             : $USE_NLS"
echo ""
echo "******************************************************************"
echo ""
echo "Looks good."
echo ""
