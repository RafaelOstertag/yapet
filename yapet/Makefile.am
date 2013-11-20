# $Id$

SUBDIRS  = ui glue

bin_PROGRAMS = yapet

yapet_LDFLAGS = -static -L$(top_srcdir)/crypt -L$(srcdir)/glue -L$(srcdir)/ui

yapet_SOURCES = main.cc cfgfile.cc consts.cc cfg.cc globals.cc	\
cfgfile.h consts.h cfg.h globals.h

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(srcdir)/glue	\
-I$(srcdir)/ui -I$(top_srcdir)/libyacurs/include	\
-DLOCALEDIR=\"$(localedir)\"

yapet_LDADD = -lyapet-glue -lyapet-ui	\
-lyapet-crypt					\
$(top_builddir)/libyacurs/src/libyacurs.la @LIBINTL@

if BUILDPWGEN
yapet_CPPFLAGS += -I$(top_srcdir)/pwgen
yapet_LDADD += -lyapet-pwgen
yapet_LDFLAGS += -L$(top_srcdir)/pwgen
endif

if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
