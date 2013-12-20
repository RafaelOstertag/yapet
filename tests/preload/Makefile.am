# $Id$

lib_LTLIBRARIES = libpwrecord.la
bin_PROGRAMS = checktestpwrecord

checktestpwrecord_SOURCES = checktestpwrecord.cc		 \
../../crypt/bdbuffer.cc ../../crypt/crypt.cc ../../crypt/file.cc \
../../crypt/key.cc ../../crypt/partdec.cc
checktestpwrecord_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_builddir) \
-I$(top_srcdir)
checktestpwrecord_LDADD = $(top_builddir)/crypt/libyapet-crypt.la

libpwrecord_la_SOURCES = pwrecord.cc
libpwrecord_la_CPPFLAGS = -I$(top_srcdir)/libyacurs/include	\
-I$(top_builddir)/libyacurs/include

libpwrecord_la_LDFLAGS  = -no-install

install:

check-local: checktestpwrecord
	rm -f /tmp/testpwrecord.pet
	$(top_builddir)/libtool --mode=execute env LD_PRELOAD=$(builddir)/.libs/libpwrecord.so $(top_builddir)/yapet/yapet -i /tmp/testpwrecord.pet
	$(top_builddir)/libtool --mode=execute $(builddir)/checktestpwrecord || exit 1
