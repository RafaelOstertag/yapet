# $Id$

DIST_SUBDIRS  = ui glue

if BUILDPWGEN
SUBDIRS = pwgen
endif

bin_PROGRAMS = yapet

ui_SOURCES = ui/help.cc ui/mainwindow.cc ui/passworddialog.cc ui/passwordrecord.cc ui/newpassworddialog.cc ui/info.cc

glue_SOURCES = glue/loadfile.cc glue/promptpassword.cc glue/createfile.cc glue/changepassword.cc

yapet_SOURCES = main.cc cfgfile.cc consts.cc cfg.cc globals.cc $(ui_SOURCES) $(glue_SOURCES)

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(srcdir)/glue -I$(srcdir)/ui -I$(top_srcdir)/libyacurs/include -DLOCALEDIR=\"$(localedir)\"

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
