yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/cfg \
    -I$(yapet_src_libdir)/globals \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/metadata \
    -I$(yapet_src_libdir)/exceptions \
    -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/passwordrecord \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir)/libyacurs/include \
    -I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    -I$(yapet_src_libdir) \
    $(OPENSSL_INCLUDES) \
    $(ARGON2_CFLAGS) \
    -DLOCALEDIR=\"$(localedir)\"
LDADD = $(yapet_libs_builddir)/file/libyapet-file.la \
    $(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la \
    $(yapet_libs_builddir)/utils/libyapet-utils.la \
    $(yapet_libs_builddir)/globals/libyapet-globals.la \
    $(yapet_libs_builddir)/cfg/libyapet-cfg.la
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(ARGON2_LIBS)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-crypt.la

libyapet_crypt_la_SOURCES = file.hh key.hh key448.hh key448.cc key256.hh key256.cc file.cc blowfish.hh blowfish.cc aes256.hh aes256.cc		\
crypto.hh crypto.cc abstractcryptofactory.hh blowfishfactory.hh blowfishfactory.cc aes256factory.hh \
aes256factory.cc cryptofactoryhelper.hh cryptofactoryhelper.cc
