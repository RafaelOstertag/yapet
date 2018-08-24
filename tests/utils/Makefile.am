check_PROGRAMS = securearray
TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/utils
AM_LDFLAGS = $(top_builddir)/utils/libyapet-utils.la $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

securearray_SOURCES = securearray.cc