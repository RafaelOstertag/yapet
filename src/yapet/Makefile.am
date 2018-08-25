# $Id$

yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_libdir = $(top_builddir)/src/libs

bin_PROGRAMS = yapet

yapet_LDFLAGS = $(LIBINTL)
yapet_SOURCES = main.cc cfgfile.cc cfg.cc globals.cc cfgfile.h	\
cfg.h globals.h


yapet_CPPFLAGS = -I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/glue	\
	-I$(yapet_libs_srcdir)/ui \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/consts \
	-I$(top_srcdir)/libyacurs/include		\
	-I$(top_builddir)/libyacurs/include	\
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\" \
	$(OPENSSL_INCLUDES)

yapet_LDADD =  $(yapet_libdir)/file/libyapet-file.la \
	$(yapet_libdir)/ui/libyapet-ui.la \
	$(yapet_libdir)/glue/libyapet-glue.la \
	$(yapet_libdir)/pwgen/libyapet-pwgen.la \
	$(yapet_libdir)/crypt/libyapet-crypt.la \
	$(yapet_libdir)/consts/libyapet-consts.la \
	$(yapet_libdir)/utils/libyapet-utils.la \
	$(top_builddir)/libyacurs/src/libyacurs.la


if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
