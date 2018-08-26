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
if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)

noinst_LTLIBRARIES = libyapet-export.la
libyapet_export_la_SOURCES = csvexport.h csvexport.cc
libyapet_export_la_LIBADD = $(yapet_libdir)/crypt/libyapet-crypt.la \
    $(yapet_libdir)/consts/libyapet-consts.la

bin_PROGRAMS = yapet2csv
yapet2csv_SOURCES = main.cc
yapet2csv_LDADD = $(builddir)/libyapet-export.la
 

