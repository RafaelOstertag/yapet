# $Id$

SUBDIRS = glue ui

if BUILDPWGEN
SUBDIRS += pwgen
endif

bin_PROGRAMS = yapet

yapet_SOURCES = main.cc cfgfile.cc consts.cc cfg.cc globals.cc

# statusbar.h fileopen.h passworddialog.h		\
# passwordrecord.h   statusbar.cc fileopen.cc			\
# searchdialog.h searchdialog.cc 	\
#  lockscreen.h lockscreen.cc

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(srcdir)/glue -I$(srcdir)/ui -I$(top_srcdir)/libyacurs/include -DLOCALEDIR=\"$(localedir)\"
yapet_LDADD = $(top_builddir)/crypt/libgpcrypt.a $(builddir)/ui/libui.a $(builddir)/glue/libglue.a $(top_builddir)/libyacurs/src/libyacurs.la @LIBINTL@

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
