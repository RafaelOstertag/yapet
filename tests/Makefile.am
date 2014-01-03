# $Id$

# Tests may fail if not run serial
AUTOMAKE_OPTIONS = serial-tests

SUBDIRS = preload

# For silent rules
cpy_verbose = $(cpy_verbose_$(V))
cpy_verbose_ = $(cpy_verbose_$(AM_DEFAULT_VERBOSITY))
cpy_verbose_0 = @echo "  CPY    $@";
# For silent rules
chmod_verbose = $(chmod_verbose_$(V))
chmod_verbose_ = $(chmod_verbose_$(AM_DEFAULT_VERBOSITY))
chmod_verbose_0 = @echo "  CHMOD  $@";

CLEANFILES = testfile.gps encryptiontest.gps encryptiontest.gps.bak	\
test1.pet test2.pet test3.pet test4.pet test5.pet test6.pet test7.pet	\
test8.pet test9.pet test10.pet endianess.pet corrupt.pet f32le0.5.pet	\
f32be0.5.pet f64le0.5.pet f64be0.5.pet f32le0.6.pet f32be0.6.pet	\
f64le0.6.pet f64be0.6.pet
#
# Files used for foreign test
#
# f32le0.5.pet.in	file created on 32bit little endian arch with a pre 0.6 version
# f32be0.5.pet.in	file created on 32bit big endian arch with a pre 0.6 version
# f64le0.5.pet.in	file created on 64bit little endian arch with a pre 0.6 version
# f64be0.5.pet.in	file created on 64bit big endian arch with a pre 0.6 version
#
# f32le0.6.pet.in	file created on 32bit little endian arch with a 0.6 version
# f32be0.6.pet.in	file created on 32bit big endian arch with a 0.6 version
# f64le0.6.pet.in	file created on 64bit little endian arch with a 0.6 version
# f64be0.6.pet.in	file created on 64bit big endian arch with a 0.6 version
EXTRA_DIST = endianess.test corrupt.pet.in f32le0.5.pet.in		\
f32be0.5.pet.in f64le0.5.pet.in f64be0.5.pet.in f32le0.6.pet.in		\
f32be0.6.pet.in f64le0.6.pet.in f64be0.6.pet.in test1.csv test2.csv	\
test3.csv test4.csv test5.csv test6.csv test7.csv test8.csv test9.csv	\
test10.csv cfgfile1.rc cfgfile2.rc cfgfile3.rc testimport.csv

# Listed as _DEPENDENCIES for corrupt and foreign
.pet.in.pet:
	$(cpy_verbose)cp $< $(builddir)/$@
	$(chmod_verbose)chmod u=rw $(builddir)/$@

check_PROGRAMS = endianess key record bdbuffer enc partdec file file2	\
file3 file4 file5 file6 corrupt foreign cfg1 cfgfile1 charpool1		\
charpool2 rng1 pwgen1

# Checks when building csv2yapet
if BUILDCSV2YAPET
check_PROGRAMS += import1 import2 import3 import4 import5 import6 import7	\
import8 import9 import10 import11 import12 import13 import14 import15
endif

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/crypt -I$(top_srcdir)/pwgen
if BUILDCSV2YAPET
AM_CPPFLAGS += -I$(top_srcdir)/csv2yapet
endif

corrupt_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
corrupt_SOURCES = corrupt.cc tests.h
corrupt_DEPENDENCIES = corrupt.pet

foreign_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
foreign_SOURCES = foreign.cc tests.h
foreign_DEPENDENCIES = f32le0.5.pet f32be0.5.pet f64le0.5.pet f64be0.5.pet	\
f32le0.6.pet f32be0.6.pet f64le0.6.pet f64be0.6.pet

endianess_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
endianess_SOURCES = endianess.cc

key_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
key_SOURCES = key.cc tests.h

record_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
record_SOURCES = record.cc tests.h

bdbuffer_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
bdbuffer_SOURCES = bdbuffer.cc tests.h

enc_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
enc_SOURCES = enc.cc tests.h

partdec_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
partdec_SOURCES = partdec.cc tests.h

file_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file_SOURCES = file.cc tests.h

file2_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file2_SOURCES = file2.cc tests.h

file3_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file3_SOURCES = file3.cc tests.h

file4_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file4_SOURCES = file4.cc tests.h

file5_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file5_SOURCES = file5.cc tests.h

file6_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
file6_SOURCES = file6.cc tests.h

cfg1_LDADD = $(LIBINTL)
cfg1_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/yapet	\
-I$(top_srcdir)/pwgen
cfg1_SOURCES = cfg1.cc ../yapet/cfg.cc	\
../yapet/consts.cc

cfgfile1_LDADD = $(LIBINTL)
cfgfile1_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/yapet
cfgfile1_SOURCES = cfgfile1.cc ../yapet/cfg.cc ../yapet/cfgfile.cc	\
../yapet/consts.cc

if BUILDCSV2YAPET

import1_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import1_SOURCES = import1.cc ../csv2yapet/csvimport.cc

import2_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import2_SOURCES = import2.cc ../csv2yapet/csvimport.cc

import3_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import3_SOURCES = import3.cc ../csv2yapet/csvimport.cc

import4_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import4_SOURCES = import4.cc ../csv2yapet/csvimport.cc

import5_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import5_SOURCES = import5.cc  ../csv2yapet/csvimport.cc

import6_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import6_SOURCES = import6.cc  ../csv2yapet/csvimport.cc

import7_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import7_SOURCES = import7.cc  ../csv2yapet/csvimport.cc

import8_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import8_SOURCES = import8.cc  ../csv2yapet/csvimport.cc

import9_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import9_SOURCES = import9.cc  ../csv2yapet/csvimport.cc

import10_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import10_SOURCES = import10.cc  ../csv2yapet/csvimport.cc

import11_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import11_SOURCES = import11.cc  ../csv2yapet/csvimport.cc

import12_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import12_SOURCES = import12.cc  ../csv2yapet/csvimport.cc

import13_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import13_SOURCES = import13.cc  ../csv2yapet/csvimport.cc

import14_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import14_SOURCES = import14.cc  ../csv2yapet/csvimport.cc

import15_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)
import15_SOURCES = import15.cc  ../csv2yapet/csvimport.cc

check-local:
	rm -f /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	$(top_builddir)/csv2yapet/csv2yapet -p secret -s ',' $(srcdir)/testimport.csv /tmp/yapettestimport.pet
	$(top_builddir)/yapet2csv/yapet2csv -p secret -s ',' /tmp/yapettestimport.pet /tmp/yapettestexport.csv
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

clean-local:
	rm -rf SunWS_cache

SUFFIXES = .pet .pet.in
