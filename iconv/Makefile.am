# $Id$

noinst_LTLIBRARIES = libyapet-iconv.la

libyapet_iconv_la_SOURCES = yaiconv.h yaiconv.cc

libyapet_iconv_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
libyapet_iconv_la_CPPFLAGS += -DNDEBUG
else
libyapet_iconv_la_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
