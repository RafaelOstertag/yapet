yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_libs_builddir = $(top_builddir)/src/libs

check_PROGRAMS = securearray utils
TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(yapet_libs_srcdir)/utils
AM_LDFLAGS = $(yapet_libs_builddir)/utils/libyapet-utils.la $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

securearray_SOURCES = securearray.cc
utils_SOURCES = utils.cc