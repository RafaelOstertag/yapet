yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir) \
    -I$(yapet_src_libdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-metadata.la
libyapet_metadata_la_SOURCES = metadata.hh metadata.cc
