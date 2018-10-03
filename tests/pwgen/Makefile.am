yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

check_PROGRAMS = characterpool rng pwgen charpool1 charpool2 rng1 pwgen1

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/utils \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(top_srcdir)
LDADD = $(LIBINTL) $(CPPUNIT_LIBS)
AM_LDFLAGS = $(yapet_libs_builddir)/pwgen/libyapet-pwgen.la $(yapet_libs_builddir)/utils/libyapet-utils.la
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

charpool1_SOURCES = charpool1.cc
charpool2_SOURCES = charpool2.cc
rng1_SOURCES = rng1.cc
pwgen1_SOURCES = pwgen1.cc
characterpool_SOURCES = characterpool.cc
rng_SOURCES = rng.cc
pwgen_SOURCES = pwgen.cc