# $Id$

noinst_LTLIBRARIES = libyapet-ui.la

libyapet_ui_la_SOURCES = help.cc mainwindow.cc passworddialog.cc	\
passwordrecord.cc newpassworddialog.cc info.cc yapetunlockdialog.cc	\
yapetlockscreen.cc help.h mainwindow.h passworddialog.h			\
passwordrecord.h newpassworddialog.h info.h yapetunlockdialog.h		\
yapetlockscreen.h pwgendialog.h pwgendialog.cc

libyapet_ui_la_LDFLAGS = -static -L$(top_builddir)/glue	\
-L$(top_builddir)/pwgen
libyapet_ui_la_LIBADD = -lyapet-glue -lyapet-pwgen

libyapet_ui_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"		 \
-I$(top_srcdir)/libyacurs/include -I$(top_srcdir)/yapet		 \
-I$(top_srcdir)/crypt -I$(top_srcdir)/glue -I$(top_srcdir)/pwgen \
-I$(top_srcdir) -I$(top_builddir)/libyacurs/include

if DISABLE_DEBUG
libyapet_ui_la_CPPFLAGS += -DNDEBUG
else
libyapet_ui_la_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
