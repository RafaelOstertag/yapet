# $Id$

noinst_LIBRARIES = libgpcrypt.a

libgpcrypt_a_SOURCES = structs.h record.h key.h yapetexception.h file.h crypt.h	\
bdbuffer.h partdec.h key.cc file.cc crypt.cc bdbuffer.cc partdec.cc

libgpcrypt_a_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
libgpcrypt_a_CPPFLAGS += -I$(top_srcdir)/intl 
endif

if DISABLE_ASSERT
libgpcrypt_a_CPPFLAGS += -DNDEBUG
endif

clean-local:
	rm -rf SunWS_cache
