yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

# Tests may fail if not run serial
AUTOMAKE_OPTIONS = serial-tests

CLEANFILES = test1.pet test2.pet test3.pet test4.pet test5.pet test6.pet test7.pet	\
test8.pet test9.pet test10.pet

EXTRA_DIST = test1.csv test2.csv	\
test3.csv test4.csv test5.csv test6.csv test7.csv test8.csv test9.csv	\
test10.csv testpaths.h.in

check_PROGRAMS = import1 import2 import3 import4 import5 import6 import7	\
import8 import9 import10 import11 import12 import13 import14 import15

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/consts \
	-I$(yapet_libs_srcdir)/exceptions \
	-I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/file \
	-I$(yapet_libs_srcdir)/utils \
	-I$(yapet_libs_srcdir)/interfaces \
	-I$(yapet_libs_srcdir)/passwordrecord \
    -I$(yapet_srcdir)/converters/csv2yapet \
	-I$(srcdir)/.. \
	-I$(top_srcdir)

LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la \
    $(yapet_builddir)/converters/csv2yapet/libyapet-import.la \
	$(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la

AM_LDFLAGS = $(LIBINTL)

import1_SOURCES = import1.cc 
import2_SOURCES = import2.cc
import3_SOURCES = import3.cc
import4_SOURCES = import4.cc
import5_SOURCES = import5.cc
import6_SOURCES = import6.cc
import7_SOURCES = import7.cc
import8_SOURCES = import8.cc
import9_SOURCES = import9.cc
import10_SOURCES = import10.cc
import11_SOURCES = import11.cc
import12_SOURCES = import12.cc
import13_SOURCES = import13.cc
import14_SOURCES = import14.cc
import15_SOURCES = import15.cc
