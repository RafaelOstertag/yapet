yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_libdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/exceptions \
	-I$(yapet_src_libdir)/crypt	\
	-I$(yapet_src_libdir)/consts \
	-I$(yapet_src_libdir)/pwgen \
	-I$(yapet_src_libdir)/utils \
	-I$(yapet_src_libdir)/file \
	-I$(yapet_src_libdir)/interfaces \
	-I$(yapet_src_libdir)/passwordrecord \
	-I$(top_srcdir) \
	$(OPENSSL_INCLUDES) \
	-DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-import.la
libyapet_import_la_SOURCES = csvstringfield.hh csvstringfield.cc csvstring.cc csvimport.h csvimport.cc
libyapet_import_la_LIBADD = $(yapet_libdir)/crypt/libyapet-crypt.la	\
	$(yapet_libdir)/consts/libyapet-consts.la			\
	$(yapet_libdir)/file/libyapet-file.la				\
	$(yapet_libdir)/utils/libyapet-utils.la

bin_PROGRAMS = csv2yapet
csv2yapet_SOURCES = main.cc
csv2yapet_LDADD = $(builddir)/libyapet-import.la \
	$(yapet_libdir)/passwordrecord/libyapet-passwordrecord.la
