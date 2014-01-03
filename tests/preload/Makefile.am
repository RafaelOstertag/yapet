# $Id$

# Building preload library has been taken out of the regular build
# flow, so that we don't have to rely on the noinst_ prefix, which
# doesn't produce a shared library, but a static one unable to be used
# by LD_PRELOAD. However, not using noinst_ prefix, a shared library
# will be built which is also installed. That we don't want either.

EXTRA_DIST = pwrecord.cc

noinst_PROGRAMS = checktestpwrecord

checktestpwrecord_SOURCES = checktestpwrecord.cc		 \
../../crypt/bdbuffer.cc ../../crypt/crypt.cc ../../crypt/file.cc \
../../crypt/key.cc ../../crypt/partdec.cc
checktestpwrecord_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_builddir) \
-I$(top_srcdir)
checktestpwrecord_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)

myCPPFLAGS = -I$(top_srcdir)/libyacurs/include	\
-I$(top_builddir)/libyacurs/include

##
## libpwrecord.so
##
pwrecord.lo: pwrecord.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) -shared $(myCPPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -c -o $(builddir)/$@ $<

libpwrecord.la: pwrecord.lo
	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath $(libdir) -version-info 0 -o $@ $<

check-local: libpwrecord.la checktestpwrecord
	rm -f /tmp/testpwrecord.pet
	$(top_builddir)/libtool --mode=execute /usr/bin/env LD_PRELOAD="$(builddir)/.libs/libpwrecord.so" $(top_builddir)/yapet/yapet -i /tmp/testpwrecord.pet
	$(top_builddir)/libtool --mode=execute $(builddir)/checktestpwrecord || exit 1

clean-local:
	rm -f *.la
	rm -f *.o
