check_PROGRAMS = securearray

TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/file

securearray_LDADD = $(top_builddir)/file/libyapet-file.la $(CPPUNIT_LIBS)
securearray_CXXFLAGS =  $(CPPUNIT_CFLAGS)
securearray_SOURCES = securearray.cc

