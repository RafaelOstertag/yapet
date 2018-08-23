check_PROGRAMS = securearray ods rawfile fileutils yapetfile yapet10file

TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/file
AM_LDFLAGS = $(top_builddir)/file/libyapet-file.la $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

securearray_SOURCES = securearray.cc
ods_SOURCES = ods.cc
rawfile_SOURCES = rawfile.cc
fileutils_SOURCES = fileutils.cc
yapetfile_SOURCES = yapetfile.cc
yapet10file_SOURCES = yapet10file.cc