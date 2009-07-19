# $Id$

bin_PROGRAMS = csv2yapet

csv2yapet_SOURCES = main.cc csvimport.h csvimport.cc $(top_srcdir)/yapet/consts.cc

csv2yapet_CPPFLAGS = -I$(top_srcdir)/yapet -I$(top_srcdir)/crypt -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
csv2yapet_CPPFLAGS += -I$(top_srcdir)/intl 
endif

csv2yapet_LDADD = $(top_builddir)/crypt/libgpcrypt.a @LIBINTL@

clean-local:
	rm -rf SunWS_cache
