# $Id$

# Building preload library has been taken out of the regular build
# flow, so that we don't have to rely on the noinst_ prefix, which
# doesn't produce a shared library, but a static one unable to be used
# by LD_PRELOAD. However, not using noinst_ prefix, a shared library
# will be built which is also installed. That we don't want either.

yapet_srcdir = $(top_srcdir)/src
yapet_lib_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs = $(yapet_builddir)/libs


EXTRA_DIST = istty.cc pwrecord.cc checkyapet.cc masterpwchange.cc

noinst_PROGRAMS = checktestpwrecord checkmasterpwchange istty

checktestpwrecord_SOURCES = checktestpwrecord.cc
checktestpwrecord_CPPFLAGS = -I$(yapet_lib_srcdir)/crypt \
	-I$(yapet_lib_srcdir)/file \
	-I$(yapet_lib_srcdir)/utils \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	 $(OPENSSL_INCLUDES)
checktestpwrecord_LDADD = $(yapet_libs)/crypt/libyapet-crypt.la \
	$(yapet_libs)/file/libyapet-file.la \
	$(yapet_libs)/utils/libyapet-utils.la \
	$(LIBINTL) 

checkmasterpwchange_SOURCES = checkmasterpwchange.cc
checkmasterpwchange_CPPFLAGS = -I$(yapet_lib_srcdir)/crypt \
	-I$(yapet_lib_srcdir)/crypt \
	-I$(yapet_lib_srcdir)/file \
	-I$(yapet_lib_srcdir)/utils \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	$(OPENSSL_INCLUDES)
checkmasterpwchange_LDADD = $(yapet_libs)/crypt/libyapet-crypt.la \
	$(yapet_libs)/file/libyapet-file.la \
	$(yapet_libs)/utils/libyapet-utils.la \
	$(LIBINTL)


istty_SOURCES = istty.cc

myCPPFLAGS = -I$(top_srcdir)/libyacurs/include	\
-I$(top_builddir)/libyacurs/include

##
## libcheckyapet.so
##
checkyapet.lo: checkyapet.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) -shared $(myCPPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -c -o $(builddir)/$@ $<

libcheckyapet.la: checkyapet.lo
	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath $(libdir) -version-info 0 -o $@ $<

##
## libpwrecord.so
##
pwrecord.lo: pwrecord.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) -shared $(myCPPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -c -o $(builddir)/$@ $<

libpwrecord.la: pwrecord.lo
	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath $(libdir) -version-info 0 -o $@ $<

##
## libmasterpwchange.so
##
masterpwchange.lo: masterpwchange.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) -shared $(myCPPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -c -o $(builddir)/$@ $<

libmasterpwchange.la: masterpwchange.lo
	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath $(libdir) -version-info 0 -o $@ $<

check-local: libpwrecord.la checktestpwrecord libmasterpwchange.la libcheckyapet.la
	if $(builddir)/istty ; then \
		rm -f /tmp/emptyfile.pet ; \
		$(top_builddir)/libtool --mode=execute /usr/bin/env LD_PRELOAD="$(builddir)/.libs/libcheckyapet.so" $(yapet_builddir)/yapet/yapet -i /tmp/emptyfile.pet && echo 'OK' ; \
		rm -f /tmp/testpwrecord.pet ; \
		$(top_builddir)/libtool --mode=execute /usr/bin/env LD_PRELOAD="$(builddir)/.libs/libpwrecord.so" $(yapet_builddir)/yapet/yapet -i /tmp/testpwrecord.pet ; \
		$(top_builddir)/libtool --mode=execute $(builddir)/checktestpwrecord || exit 1 ; \
		rm -f /tmp/masterpwchange.pet ; \
		$(top_builddir)/libtool --mode=execute /usr/bin/env LD_PRELOAD="$(builddir)/.libs/libmasterpwchange.so" $(yapet_builddir)/yapet/yapet -i /tmp/masterpwchange.pet ; \
		$(top_builddir)/libtool --mode=execute $(builddir)/checkmasterpwchange || exit 1 ; \
	fi

clean-local:
	rm -f *.la
	rm -f *.o
