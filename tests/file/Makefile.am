yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

EXTRA_DIST = testpaths.h.in yapet10file-corrupt-identifier.dat yapet20file-corrupt-identifier.dat
CLEANFILES = yapet-fileutils-test yapet-rawfile-test yapet-yapet10file-test yapet-yapet20file-test yapet-yapetfile-test

check_PROGRAMS = ods rawfile fileutils yapetfile yapet10file yapet20file header10 headerversion
TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/pwgen \
    -I$(yapet_libs_srcdir)/consts \
    -I$(yapet_libs_srcdir)/exceptions \
    -I$(yapet_libs_srcdir)/utils \
    -I$(yapet_libs_srcdir)/interfaces \
    -I$(yapet_libs_srcdir)/file \
    -I$(top_srcdir) 
LDADD = $(yapet_libs_builddir)/file/libyapet-file.la $(yapet_libs_builddir)/utils/libyapet-utils.la
AM_LDFLAGS =  $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

ods_SOURCES = ods.cc
rawfile_SOURCES = rawfile.cc
fileutils_SOURCES = fileutils.cc
yapetfile_SOURCES = yapetfile.cc
yapet10file_SOURCES = yapet10file.cc
yapet20file_SOURCES = yapet20file.cc
header10_SOURCES = header10.cc
headerversion_SOURCES = headerversion.cc