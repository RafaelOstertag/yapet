# $Id$

yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_libdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/crypt	\
	-I$(yapet_src_libdir)/consts \
	-I$(yapet_src_libdir)/pwgen \
	-I$(top_srcdir) \
	$(OPENSSL_INCLUDES) \
	-DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-import.la
libyapet_import_la_SOURCES = csvimport.h csvimport.cc
libyapet_import_la_LIBADD = $(yapet_libdir)/crypt/libyapet-crypt.la \
	$(yapet_libdir)/consts/libyapet-consts.la

bin_PROGRAMS = csv2yapet
csv2yapet_SOURCES = main.cc
csv2yapet_LDADD = $(builddir)/libyapet-import.la