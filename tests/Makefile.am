# $Id$

yapet_srcdir = $(top_srcdir)/srcdir
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

# Tests may fail if not run serial
AUTOMAKE_OPTIONS = serial-tests

SUBDIRS = crypt file preload 

EXTRA_DIST = test1.csv test2.csv	\
test3.csv test4.csv test5.csv test6.csv test7.csv test8.csv test9.csv	\
test10.csv cfgfile1.rc cfgfile2.rc cfgfile3.rc testimport.csv

check_PROGRAMS = cfg1 cfgfile1 charpool1 charpool2 rng1 pwgen1

# Checks when building csv2yapet
if BUILDCONVERTS
check_PROGRAMS += import1 import2 import3 import4 import5 import6 import7	\
import8 import9 import10 import11 import12 import13 import14 import15
endif

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(builddir) \
	-I$(srcdir) \
	-I$(top_srcdir)
if BUILDCONVERTS
AM_CPPFLAGS += -I$(top_srcdir)/csv2yapet
endif

cfg1_LDADD = $(LIBINTL)
cfg1_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/yapet	\
-I$(top_srcdir)/pwgen
cfg1_SOURCES = cfg1.cc ../yapet/cfg.cc	\
../yapet/consts.cc

cfgfile1_LDADD = $(LIBINTL)
cfgfile1_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/yapet
cfgfile1_SOURCES = cfgfile1.cc ../yapet/cfg.cc ../yapet/cfgfile.cc	\
../yapet/consts.cc


if BUILDCONVERTS

import1_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import1_SOURCES = import1.cc ../csv2yapet/csvimport.cc

import2_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import2_SOURCES = import2.cc ../csv2yapet/csvimport.cc

import3_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import3_SOURCES = import3.cc ../csv2yapet/csvimport.cc

import4_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import4_SOURCES = import4.cc ../csv2yapet/csvimport.cc

import5_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import5_SOURCES = import5.cc  ../csv2yapet/csvimport.cc

import6_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import6_SOURCES = import6.cc  ../csv2yapet/csvimport.cc

import7_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import7_SOURCES = import7.cc  ../csv2yapet/csvimport.cc

import8_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import8_SOURCES = import8.cc  ../csv2yapet/csvimport.cc

import9_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import9_SOURCES = import9.cc  ../csv2yapet/csvimport.cc

import10_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import10_SOURCES = import10.cc  ../csv2yapet/csvimport.cc

import11_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import11_SOURCES = import11.cc  ../csv2yapet/csvimport.cc

import12_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import12_SOURCES = import12.cc  ../csv2yapet/csvimport.cc

import13_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import13_SOURCES = import13.cc  ../csv2yapet/csvimport.cc

import14_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import14_SOURCES = import14.cc  ../csv2yapet/csvimport.cc

import15_LDADD = $(yapet_libs_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import15_SOURCES = import15.cc  ../csv2yapet/csvimport.cc

check-local:
	rm -f /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	$(yapet_builddir)/csv2yapet/csv2yapet -p secret -s ',' $(srcdir)/testimport.csv /tmp/yapettestimport.pet
	$(yapet_builddir)/yapet2csv/yapet2csv -p secret -s ',' /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	diff $(srcdir)/testimport.csv /tmp/yapettestexport.csv

endif

charpool1_LDADD = $(LIBINTL)
charpool1_SOURCES = charpool1.cc ../pwgen/charpool.cc	\
../pwgen/pwgen.cc ../pwgen/rng.cc

charpool2_LDADD = $(LIBINTL)
charpool2_SOURCES = charpool2.cc ../pwgen/charpool.cc	\
../pwgen/pwgen.cc ../pwgen/rng.cc

rng1_LDADD = $(LIBINTL)
rng1_SOURCES = rng1.cc ../pwgen/charpool.cc ../pwgen/pwgen.cc	\
../pwgen/rng.cc

pwgen1_LDADD = $(LIBINTL)
pwgen1_SOURCES = pwgen1.cc ../pwgen/charpool.cc ../pwgen/pwgen.cc	\
../pwgen/rng.cc
