yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/cfg \
    -I$(yapet_src_libdir)/pwgen \
    -I$(top_srcdir)/libyacurs/include \
    -I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\" 

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-globals.la

libyapet_globals_la_SOURCES = globals.cc globals.h