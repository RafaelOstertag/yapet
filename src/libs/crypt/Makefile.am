yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir) \
    $(OPENSSL_INCLUDES) \
    -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-crypt.la

libyapet_crypt_la_SOURCES = structs.h yapetexception.h	\
file.h partdec.h key448.hh key448.cc file.cc blowfish.hh blowfish.cc		\
partdec.cc crypto.hh crypto.cc