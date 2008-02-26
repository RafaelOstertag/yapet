# $Id$

DISTCLEANFILES = testfile.gps encryptiontest.gps encryptiontest.gps.bak

check_PROGRAMS = key record bdbuffer enc partdec file file2 file3 file4 file5

TESTS = $(check_PROGRAMS)

key_CXXFLAGS = -I$(top_srcdir)/crypt
key_LDADD = $(top_builddir)/crypt/libgpcrypt.a
key_SOURCES = key.cc tests.h

record_CXXFLAGS = -I$(top_srcdir)/crypt
record_LDADD = $(top_builddir)/crypt/libgpcrypt.a
record_SOURCES = record.cc tests.h

bdbuffer_CXXFLAGS = -I$(top_srcdir)/crypt
bdbuffer_LDADD = $(top_builddir)/crypt/libgpcrypt.a
bdbuffer_SOURCES = bdbuffer.cc tests.h

enc_CXXFLAGS = -I$(top_srcdir)/crypt
enc_LDADD = $(top_builddir)/crypt/libgpcrypt.a
enc_SOURCES = enc.cc tests.h

partdec_CXXFLAGS = -I$(top_srcdir)/crypt
partdec_LDADD = $(top_builddir)/crypt/libgpcrypt.a
partdec_SOURCES = partdec.cc tests.h

file_CXXFLAGS = -I$(top_srcdir)/crypt
file_LDADD = $(top_builddir)/crypt/libgpcrypt.a
file_SOURCES = file.cc tests.h

file2_CXXFLAGS = -I$(top_srcdir)/crypt
file2_LDADD = $(top_builddir)/crypt/libgpcrypt.a
file2_SOURCES = file2.cc tests.h

file3_CXXFLAGS = -I$(top_srcdir)/crypt
file3_LDADD = $(top_builddir)/crypt/libgpcrypt.a
file3_SOURCES = file3.cc tests.h

file4_CXXFLAGS = -I$(top_srcdir)/crypt
file4_LDADD = $(top_builddir)/crypt/libgpcrypt.a
file4_SOURCES = file4.cc tests.h

file5_CXXFLAGS = -I$(top_srcdir)/crypt
file5_LDADD = $(top_builddir)/crypt/libgpcrypt.a
file5_SOURCES = file5.cc tests.h
