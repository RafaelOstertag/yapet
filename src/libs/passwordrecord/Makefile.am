yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-passwordrecord.la
libyapet_passwordrecord_la_SOURCES = passwordrecord.hh passwordrecord.cc \
    passwordlistitem.hh passwordlistitem.cc
