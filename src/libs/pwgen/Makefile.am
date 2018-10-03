yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/utils \ 
    -I$(yapet_src_libdir)/consts \
    -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-pwgen.la

libyapet_pwgen_la_SOURCES = rng.cc rng.hh	\
pwgen.cc pwgen.hh characterpool.hh characterpool.cc
