yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_libdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/exceptions \
	-I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/glue	\
	-I$(yapet_libs_srcdir)/ui \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/consts \
	-I$(yapet_libs_srcdir)/cfg \
	-I$(yapet_libs_srcdir)/file \
	-I$(yapet_libs_srcdir)/utils \
	-I$(yapet_libs_srcdir)/interfaces \
	-I$(yapet_libs_srcdir)/passwordrecord \
	-I$(top_srcdir)/libyacurs/include		\
	-I$(top_builddir)/libyacurs/include	\
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\" \
	$(OPENSSL_INCLUDES)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

LDADD = $(yapet_libdir)/ui/libyapet-ui.la				\
	$(yapet_libdir)/glue/libyapet-glue.la				\
	$(yapet_libdir)/pwgen/libyapet-pwgen.la				\
	$(yapet_libdir)/crypt/libyapet-crypt.la				\
	$(yapet_libdir)/file/libyapet-file.la				\
	$(yapet_libdir)/consts/libyapet-consts.la			\
	$(yapet_libdir)/utils/libyapet-utils.la				\
	$(yapet_libdir)/cfg/libyapet-cfg.la				\
	$(yapet_libdir)/passwordrecord/libyapet-passwordrecord.la	\
	$(top_builddir)/libyacurs/src/libyacurs.la

AM_LDFLAGS = $(LIBINTL)

bin_PROGRAMS = yapet

yapet_SOURCES = main.cc globals.cc globals.h
