# $Id$

noinst_LTLIBRARIES = libyapet-ui.la

libyapet_ui_la_SOURCES = help.cc mainwindow.cc passworddialog.cc	\
passwordrecord.cc newpassworddialog.cc info.cc yapetunlockdialog.cc	\
yapetlockscreen.cc help.h mainwindow.h passworddialog.h			\
passwordrecord.h newpassworddialog.h info.h yapetunlockdialog.h		\
yapetlockscreen.h

libyapet_ui_la_LDFLAGS = -static -L$(top_srcdir)/yapet/glue
libyapet_ui_la_LIBADD = -lyapet-glue

libyapet_ui_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/libyacurs/include -I$(top_srcdir)/yapet -I$(top_srcdir)/crypt -I$(top_srcdir)/yapet/glue

if BUILDPWGEN
libyapet_ui_la_SOURCES += pwgendialog.h pwgendialog.cc
libyapet_ui_la_CPPFLAGS += -I$(top_srcdir)/pwgen
libyapet_ui_la_LIBADD += -lyapet-pwgen
libyapet_ui_la_LDFLAGS += -L$(top_srcdir)/pwgen
endif

if DISABLE_DEBUG
libyapet_ui_la_CPPFLAGS += -DNDEBUG
else
libyapet_ui_la_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
