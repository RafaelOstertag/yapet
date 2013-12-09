# $Id$

bin_PROGRAMS = yapet

yapet_LDFLAGS = -static -L$(top_srcdir)/crypt -L$(top_srcdir)/glue	\
-L$(top_srcdir)/ui -L$(top_srcdir)/pwgen

yapet_SOURCES = main.cc cfgfile.cc consts.cc cfg.cc globals.cc	\
cfgfile.h consts.h cfg.h globals.h

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/glue	\
-I$(top_srcdir)/ui -I$(top_srcdir)/libyacurs/include		\
-I$(top_srcdir)/pwgen -DLOCALEDIR=\"$(localedir)\"

yapet_LDADD = -lyapet-glue -lyapet-ui -lyapet-crypt -lyapet-pwgen	\
$(top_builddir)/libyacurs/src/libyacurs.la @LIBINTL@

if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
