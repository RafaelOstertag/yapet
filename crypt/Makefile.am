# $Id$

noinst_LTLIBRARIES = libyapet-crypt.la

libyapet_crypt_la_SOURCES = structs.h record.h key.h yapetexception.h	\
file.h crypt.h bdbuffer.h partdec.h key.cc file.cc crypt.cc		\
bdbuffer.cc partdec.cc

libyapet_crypt_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
libyapet_crypt_la_CPPFLAGS += -I$(top_builddir)/intl 
endif

if DISABLE_DEBUG
libyapet_crypt_la_CPPFLAGS += -DNDEBUG
else
libyapet_crypt_la_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
