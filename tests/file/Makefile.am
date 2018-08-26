yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

# Tests may fail if not run serial
AUTOMAKE_OPTIONS = serial-tests

EXTRA_DIST = testpaths.h.in

check_PROGRAMS = ods rawfile fileutils yapetfile yapet10file

TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(yapet_libs_srcdir)/utils -I$(yapet_libs_srcdir)/file
LDADD = $(yapet_libs_builddir)/file/libyapet-file.la $(yapet_libs_builddir)/utils/libyapet-utils.la
AM_LDFLAGS =  $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

ods_SOURCES = ods.cc
rawfile_SOURCES = rawfile.cc
fileutils_SOURCES = fileutils.cc
yapetfile_SOURCES = yapetfile.cc
yapet10file_SOURCES = yapet10file.cc