yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(top_srcdir)/src/yapet \
    -I$(yapet_src_libdir)/exceptions \
    -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/metadata \
    -I$(yapet_src_libdir)/crypt \
    -I$(yapet_src_libdir)/glue \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/cfg \
    -I$(yapet_src_libdir)/file \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(yapet_src_libdir)/passwordrecord \
    -I$(top_srcdir)/libyacurs/include		\
	-I$(top_builddir)/libyacurs/include	\
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-ui.la

libyapet_ui_la_SOURCES = help.cc mainwindow.cc passworddialog.cc	\
passwordrecord.cc newpassworddialog.cc info.cc yapetunlockdialog.cc	\
yapetlockscreen.cc help.h mainwindow.h passworddialog.h			\
passwordrecord.h newpassworddialog.h info.h yapetunlockdialog.h		\
yapetlockscreen.h pwgendialog.h pwgendialog.cc mainwindowhotkeys.h
# This resolves circular dependency between ui and glue when linked into yapet
libyapet_ui_la_LIBADD = $(yapet_libs_builddir)/glue/libyapet-glue.la
