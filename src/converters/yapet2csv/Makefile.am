# $Id$

yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_libdir = $(top_builddir)/src/libs

bin_PROGRAMS = yapet2csv

yapet2csv_SOURCES = main.cc csvexport.h csvexport.cc

yapet2csv_CPPFLAGS = -I$(yapet_src_libdir)/crypt	\
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/pwgen \
    -I$(top_srcdir) \
    $(OPENSSL_INCLUDES) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
yapet2csv_CPPFLAGS += -DNDEBUG
else
yapet2csv_CPPFLAGS += -DDEBUG
endif

yapet2csv_LDADD = $(yapet_libdir)/crypt/libyapet-crypt.la \
    $(yapet_libdir)/consts/libyapet-consts.la
    
yapet2csv_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)
