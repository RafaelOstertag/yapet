yapet_srcdir = $(top_srcdir)/src
yapet_src_libsdir = $(yapet_srcdir)/libs
yapet_build_libsdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_src_libsdir)/exceptions \
	-I$(yapet_src_libsdir)/crypt \
	-I$(yapet_src_libsdir)/metadata \
	-I$(yapet_src_libsdir)/glue	\
	-I$(yapet_src_libsdir)/ui \
	-I$(yapet_src_libsdir)/pwgen \
	-I$(yapet_src_libsdir)/consts \
	-I$(yapet_src_libsdir)/cfg \
	-I$(yapet_src_libsdir)/file \
	-I$(yapet_src_libsdir)/utils \
	-I$(yapet_src_libsdir)/interfaces \
	-I$(yapet_src_libsdir)/passwordrecord \
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

LDADD = $(yapet_build_libsdir)/ui/libyapet-ui.la				\
	$(yapet_build_libsdir)/glue/libyapet-glue.la				\
	$(yapet_build_libsdir)/pwgen/libyapet-pwgen.la				\
	$(yapet_build_libsdir)/crypt/libyapet-crypt.la				\
	$(yapet_build_libsdir)/file/libyapet-file.la				\
	$(yapet_build_libsdir)/consts/libyapet-consts.la			\
	$(yapet_build_libsdir)/utils/libyapet-utils.la				\
	$(yapet_build_libsdir)/cfg/libyapet-cfg.la				\
	$(yapet_build_libsdir)/metadata/libyapet-metadata.la				\
	$(yapet_build_libsdir)/passwordrecord/libyapet-passwordrecord.la	\
	$(top_builddir)/libyacurs/src/libyacurs.la

AM_LDFLAGS = $(LIBINTL)

bin_PROGRAMS = yapet

yapet_SOURCES = main.cc globals.cc globals.h
