yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

check_PROGRAMS = characterpool rng pwgen rng_distribution pwgen_distribution

TESTS := characterpool rng pwgen

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/utils \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(top_srcdir)
LDADD = $(LIBINTL) $(CPPUNIT_LIBS)
AM_LDFLAGS = $(yapet_libs_builddir)/pwgen/libyapet-pwgen.la $(yapet_libs_builddir)/utils/libyapet-utils.la
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

characterpool_SOURCES = characterpool.cc
rng_SOURCES = rng.cc
pwgen_SOURCES = pwgen.cc
rng_distribution_SOURCES = rng_distribution.cc
pwgen_distribution_SOURCES = pwgen_distribution.cc