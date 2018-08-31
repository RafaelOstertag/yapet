yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/utils \
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

libyapet_crypt_la_SOURCES = structs.h record.h key.h yapetexception.h	\
file.h crypt.h bdbuffer.h partdec.h key.cc file.cc crypt.cc		\
bdbuffer.cc partdec.cc