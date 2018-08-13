check_PROGRAMS = securearray ods

TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/file
AM_LDFLAGS = $(top_builddir)/file/libyapet-file.la $(CPPUNIT_LIBS)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

securearray_SOURCES = securearray.cc
ods_SOURCES = ods.cc