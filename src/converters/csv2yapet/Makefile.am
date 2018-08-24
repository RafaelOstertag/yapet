# $Id$

bin_PROGRAMS = csv2yapet

csv2yapet_SOURCES = main.cc csvimport.h csvimport.cc	\
../yapet/consts.cc

csv2yapet_CPPFLAGS = -I$(top_srcdir)/yapet -I$(top_srcdir)/crypt	\
-I$(top_srcdir) -I$(top_srcdir)/pwgen -DLOCALEDIR=\"$(localedir)\" $(OPENSSL_INCLUDES)

if DISABLE_DEBUG
csv2yapet_CPPFLAGS += -DNDEBUG
else
csv2yapet_CPPFLAGS += -DDEBUG
endif

csv2yapet_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(OPENSSL_LIBS) $(LIBINTL)
csv2yapet_LDFLAGS = $(OPENSSL_LDFLAGS)


clean-local:
	rm -rf SunWS_cache
