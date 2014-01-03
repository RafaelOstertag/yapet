# $Id$

bin_PROGRAMS = yapet2csv

yapet2csv_SOURCES = main.cc csvexport.h csvexport.cc	\
../yapet/consts.cc

yapet2csv_CPPFLAGS = -I$(top_srcdir)/yapet -I$(top_srcdir)/crypt	\
-I$(top_srcdir) -I$(top_srcdir)/pwgen -DLOCALEDIR=\"$(localedir)\" $(OPENSSL_INCLUDES)

if DISABLE_DEBUG
yapet2csv_CPPFLAGS += -DNDEBUG
else
yapet2csv_CPPFLAGS += -DDEBUG
endif

yapet2csv_LDADD = $(top_builddir)/crypt/libyapet-crypt.la $(OPENSSL_LIBS) $(LIBINTL)
yapet2csv_LDFLAGS = $(OPENSSL_LDFLAGS)
