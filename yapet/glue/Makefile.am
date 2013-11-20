# $Id$

noinst_LTLIBRARIES = libyapet-glue.la

libyapet_glue_la_SOURCES = loadfile.cc promptpassword.cc createfile.cc	\
changepassword.cc loadfile.h promptpassword.h createfile.h	\
changepassword.h

libyapet_glue_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/libyacurs/include -I$(top_srcdir)/yapet -I$(top_srcdir)/yapet/ui -I$(top_srcdir)/crypt

libyapet_glue_la_LDFLAGS = -static -L$(top_srcdir)/yapet/ui
libyapet_glue_la_LIBADD = -lyapet-ui

if DISABLE_DEBUG
libyapet_glue_la_CPPFLAGS += -DNDEBUG
else
libyapet_glue_la_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
