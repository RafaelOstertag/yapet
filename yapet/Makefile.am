# $Id$

if BUILDPWGEN
SUBDIRS = pwgen
endif

bin_PROGRAMS = yapet

yapet_SOURCES = mainwindow.h statusbar.h fileopen.h passworddialog.h		\
passwordrecord.h main.cc mainwindow.cc statusbar.cc fileopen.cc			\
passworddialog.cc passwordrecord.cc searchdialog.h searchdialog.cc cfgfile.h	\
cfgfile.cc consts.h consts.cc cfg.h cfg.cc globals.h globals.cc lockscreen.h	\
lockscreen.cc

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/ui -DLOCALEDIR=\"$(localedir)\"
yapet_LDADD = $(top_builddir)/crypt/libgpcrypt.a $(top_builddir)/ui/libui.a @LIBINTL@

if USE_INCLUDED_LIBINTL
yapet_CPPFLAGS += -I$(top_builddir)/intl 
endif

if BUILDPWGEN
yapet_CPPFLAGS += -I$(srcdir)/pwgen
yapet_LDADD += $(builddir)/pwgen/libypwgen.a
yapet_SOURCES += pwgendialog.h pwgendialog.cc
endif

if DISABLE_ASSERT
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
