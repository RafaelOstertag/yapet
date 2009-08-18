# $Id$

noinst_LIBRARIES = libypwgen.a

libypwgen_a_SOURCES = pwgenexception.h charpool.cc charpool.h rng.cc rng.h pwgen.cc pwgen.h

libypwgen_a_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
libypwgen_a_CPPFLAGS += -I$(top_srcdir)/intl 
endif

if DISABLE_ASSERT
libypwgen_a_CPPFLAGS += -DNDEBUG
else
libypwgen_a_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
