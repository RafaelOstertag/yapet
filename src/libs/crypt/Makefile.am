yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/passwordrecord \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir) \
    $(OPENSSL_INCLUDES) \
    -DLOCALEDIR=\"$(localedir)\"
LDADD = $(yapet_libs_builddir)/file/libyapet-file.la \
    $(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la \
    $(yapet_libs_builddir)/utils/libyapet-utils.la
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-crypt.la

libyapet_crypt_la_SOURCES = structs.h yapetexception.h	\
file.h key448.hh key448.cc file.cc blowfish.hh blowfish.cc		\
crypto.hh crypto.cc abstractcryptofactory.hh blowfishfactory.hh blowfishfactory.cc