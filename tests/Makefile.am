# $Id$

DISTCLEANFILES = testfile.gps encryptiontest.gps encryptiontest.gps.bak test1.pet
EXTRA_DIST = test1.csv

check_PROGRAMS = key record bdbuffer enc partdec file file2 file3 file4 file5 import1 import2

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(top_srcdir)/crypt

if USE_INCLUDED_LIBINTL
AM_CPPFLAGS += -I$(top_srcdir)/intl 
endif

key_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
key_SOURCES = key.cc tests.h

record_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
record_SOURCES = record.cc tests.h

bdbuffer_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
bdbuffer_SOURCES = bdbuffer.cc tests.h

enc_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
enc_SOURCES = enc.cc tests.h

partdec_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
partdec_SOURCES = partdec.cc tests.h

file_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
file_SOURCES = file.cc tests.h

file2_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
file2_SOURCES = file2.cc tests.h

file3_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
file3_SOURCES = file3.cc tests.h

file4_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
file4_SOURCES = file4.cc tests.h

file5_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
file5_SOURCES = file5.cc tests.h

import1_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
import1_CPPFLAGS = -I$(top_srcdir)/csv2yapet -I$(top_srcdir)/crypt
import1_SOURCES = import1.cc $(top_builddir)/csv2yapet/csvimport.h $(top_builddir)/csv2yapet/csvimport.cc

import2_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@
import2_CPPFLAGS = -I$(top_srcdir)/csv2yapet -I$(top_srcdir)/crypt
import2_SOURCES = import2.cc $(top_builddir)/csv2yapet/csvimport.h $(top_builddir)/csv2yapet/csvimport.cc

clean-local:
	rm -rf SunWS_cache
