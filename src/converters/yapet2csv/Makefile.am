yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_src_convertersdir = $(yapet_src)/converters

yapet_build_convertersdir = $(top_builddir)/src/converters
yapet_build_libdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_src_convertersdir)/csv \
    -I$(yapet_src_libdir)/globals \
    -I$(yapet_src_libdir)/exceptions \
    -I$(yapet_src_libdir)/metadata \
    -I$(yapet_src_libdir)/crypt	\
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/utils \
	-I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/interfaces \
	-I$(yapet_src_libdir)/passwordrecord \
    -I$(top_srcdir)/libyacurs/include \
	-I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    $(OPENSSL_INCLUDES) \
    -DLOCALEDIR=\"$(localedir)\"
if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

AM_LDFLAGS = $(OPENSSL_LDFLAGS) $(OPENSSL_LIBS) $(LIBINTL)

noinst_LTLIBRARIES = libyapet-export.la
libyapet_export_la_SOURCES = csvexport.h csvexport.cc
libyapet_export_la_LIBADD = $(yapet_build_libdir)/crypt/libyapet-crypt.la	\
    $(yapet_build_libdir)/consts/libyapet-consts.la				\
    $(yapet_build_libdir)/file/libyapet-file.la				\
    $(yapet_build_libdir)/utils/libyapet-utils.la           \
    $(yapet_build_libdir)/metadata/libyapet-metadata.la           \
    $(yapet_build_convertersdir)/csv/libyapet-csv.la

bin_PROGRAMS = yapet2csv
yapet2csv_SOURCES = main.cc
yapet2csv_LDADD = $(builddir)/libyapet-export.la \
    $(yapet_build_libdir)/passwordrecord/libyapet-passwordrecord.la \
    $(yapet_build_libdir)/globals/libyapet-globals.la \
	$(yapet_build_libdir)/cfg/libyapet-cfg.la \
	$(top_builddir)/libyacurs/src/libyacurs.la


