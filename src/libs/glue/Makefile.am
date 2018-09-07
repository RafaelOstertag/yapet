# $Id$

yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(top_srcdir)/src/yapet \
    -I$(yapet_src_libdir)/crypt		\
    -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/ui \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/cfg \
    -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(yapet_src_libdir)/passwordrecord \
    -I$(top_srcdir)/libyacurs/include \
    -I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-glue.la

libyapet_glue_la_SOURCES = loadfile.cc promptpassword.cc createfile.cc	\
changepassword.cc loadfile.h promptpassword.h createfile.h	\
changepassword.h opencmdlinefile.cc opencmdlinefile.h
