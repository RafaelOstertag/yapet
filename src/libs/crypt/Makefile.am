AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir) $(OPENSSL_INCLUDES)
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-crypt.la

libyapet_crypt_la_SOURCES = structs.h record.h key.h yapetexception.h	\
file.h crypt.h bdbuffer.h partdec.h key.cc file.cc crypt.cc		\
bdbuffer.cc partdec.cc