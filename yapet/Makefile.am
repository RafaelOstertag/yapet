# $Id$

bin_PROGRAMS = yapet

yapet_SOURCES = mainwindow.h statusbar.h fileopen.h passworddialog.h		\
passwordrecord.h main.cc mainwindow.cc statusbar.cc fileopen.cc			\
passworddialog.cc passwordrecord.cc searchdialog.h searchdialog.cc cfgfile.h	\
cfgfile.cc consts.h consts.cc cfg.h cfg.cc

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/ui -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
yapet_CPPFLAGS += -I$(top_srcdir)/intl 
endif


yapet_LDADD = $(top_builddir)/crypt/libgpcrypt.a $(top_builddir)/ui/libui.a @LIBINTL@

clean-local:
	rm -rf SunWS_cache
