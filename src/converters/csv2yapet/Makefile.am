# $Id$

yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_libdir = $(top_builddir)/src/libs

bin_PROGRAMS = csv2yapet

csv2yapet_SOURCES = main.cc csvimport.h csvimport.cc

csv2yapet_CPPFLAGS = -I$(yapet_src_libdir)/crypt	\
	-I$(yapet_src_libdir)/consts \
	-I$(yapet_src_libdir)/pwgen \
	-I$(top_srcdir) \
	$(OPENSSL_INCLUDES) \
	-DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
csv2yapet_CPPFLAGS += -DNDEBUG
else
csv2yapet_CPPFLAGS += -DDEBUG
endif

csv2yapet_LDADD = $(yapet_libdir)/crypt/libyapet-crypt.la \
	$(yapet_libdir)/consts/libyapet-consts.la
	
csv2yapet_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)
