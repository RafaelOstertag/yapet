yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

EXTRA_DIST = cfgfile1.rc cfgfile2.rc cfgfile3.rc testpaths.h.in

check_PROGRAMS = cfg1 cfgfile1

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/cfg \
    -I$(yapet_libs_srcdir)/pwgen \
    -I$(yapet_libs_srcdir)/consts \
	-I$(builddir) \
	-I$(srcdir) \
	-I$(top_srcdir)
LDADD = $(LIBINTL)
AM_LDFLAGS = $(yapet_libs_builddir)/cfg/libyapet-cfg.la $(yapet_libs_builddir)/pwgen/libyapet-pwgen.la $(yapet_libs_builddir)/consts/libyapet-consts.la 


cfg1_SOURCES = cfg1.cc
cfgfile1_SOURCES = cfgfile1.cc 
