# $Id$

noinst_LIBRARIES = libypwgen.a

libypwgen_a_SOURCES = pwgen.cc pwgen.h

libypwgen_a_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
libypwgen_a_CPPFLAGS += -I$(top_srcdir)/intl 
endif

if DISABLE_ASSERT
libypwgen_a_CPPFLAGS += -DNDEBUG
endif

clean-local:
	rm -rf SunWS_cache
