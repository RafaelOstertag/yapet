# $Id$

if BUILDPWGEN
SUBDIRS = pwgen
endif

bin_PROGRAMS = yapet

yapet_SOURCES = main.cc mainwindow.cc help.cc	\
cfgfile.cc consts.cc cfg.cc globals.cc	\
 passworddialog.cc passwordrecord.cc

# statusbar.h fileopen.h passworddialog.h		\
# passwordrecord.h   statusbar.cc fileopen.cc			\
# searchdialog.h searchdialog.cc 	\
#  lockscreen.h lockscreen.cc

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/libyacurs/include -DLOCALEDIR=\"$(localedir)\"
yapet_LDADD = $(top_builddir)/crypt/libgpcrypt.a $(top_builddir)/libyacurs/src/libyacurs.la @LIBINTL@

if BUILDPWGEN
yapet_CPPFLAGS += -I$(srcdir)/pwgen
yapet_LDADD += $(builddir)/pwgen/libypwgen.a
yapet_SOURCES += pwgendialog.h pwgendialog.cc
endif

if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
