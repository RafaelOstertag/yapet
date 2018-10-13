yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_converters_srcdir = $(yapet_srcdir)/converters
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs
yapet_converters_builddir = $(yapet_builddir)/converters

CLEANFILES = test1.pet test2.pet test3.pet test4.pet test5.pet test6.pet test7.pet	\
test8.pet test9.pet test10.pet

EXTRA_DIST = test1.csv test2.csv	\
test3.csv test4.csv test5.csv test6.csv test7.csv test8.csv test9.csv	\
test10.csv testpaths.h.in

check_PROGRAMS = import import2

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_converters_srcdir)/csv \
	-I$(yapet_libs_srcdir)/consts \
	-I$(yapet_libs_srcdir)/metadata \
	-I$(yapet_libs_srcdir)/exceptions \
	-I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/file \
	-I$(yapet_libs_srcdir)/utils \
	-I$(yapet_libs_srcdir)/interfaces \
	-I$(yapet_libs_srcdir)/passwordrecord \
    -I$(yapet_srcdir)/converters/csv2yapet \
	-I$(srcdir) \
	-I$(top_srcdir)

AM_CXXFLAGS = $(CPPUNIT_CFLAGS)

LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la \
    $(yapet_builddir)/converters/csv2yapet/libyapet-import.la \
	$(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la \
	$(yapet_libs_builddir)/metadata/libyapet-metadata.la \
	$(yapet_converters_builddir)/csv/libyapet-csv.la \
	$(yapet_libs_builddir)/globals/libyapet-globals.la \
	$(yapet_libs_builddir)/cfg/libyapet-cfg.la \
	$(top_builddir)/libyacurs/src/libyacurs.la

AM_LDFLAGS = $(LIBINTL) $(CPPUNIT_LIBS)

import_SOURCES = import.cc
import2_SOURCES = import2.cc
