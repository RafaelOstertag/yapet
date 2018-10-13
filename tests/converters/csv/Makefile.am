yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_converters_srcdir = $(yapet_srcdir)/converters
yapet_builddir= $(top_builddir)/src
yapet_converters_builddir= $(yapet_builddir)/converters

check_PROGRAMS = csvstringfield csvline

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_converters_srcdir)/csv
	-I$(srcdir)/.. \
	-I$(top_srcdir)

LDADD = $(yapet_converters_builddir)/csv/libyapet-csv.la

AM_LDFLAGS = $(LIBINTL) $(CPPUNIT_LIBS)

csvstringfield_SOURCES = csvstringfield.cc
csvline_SOURCES = csvline.cc