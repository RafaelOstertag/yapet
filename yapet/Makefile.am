# $Id$

bin_PROGRAMS = yapet

pwgen_sources = $(top_srcdir)/pwgen/charpool.cc		\
$(top_srcdir)/pwgen/rng.cc $(top_srcdir)/pwgen/pwgen.cc

glue_sources = $(top_srcdir)/glue/loadfile.cc				\
$(top_srcdir)/glue/promptpassword.cc $(top_srcdir)/glue/createfile.cc	\
$(top_srcdir)/glue/changepassword.cc					\
$(top_srcdir)/glue/opencmdlinefile.cc

ui_sources = $(top_srcdir)/ui/help.cc $(top_srcdir)/ui/mainwindow.cc	\
$(top_srcdir)/ui/passworddialog.cc $(top_srcdir)/ui/passwordrecord.cc	\
$(top_srcdir)/ui/newpassworddialog.cc $(top_srcdir)/ui/info.cc		\
$(top_srcdir)/ui/yapetunlockdialog.cc					\
$(top_srcdir)/ui/yapetlockscreen.cc $(top_srcdir)/ui/pwgendialog.cc

yapet_LDFLAGS = -static

yapet_SOURCES = $(pwgen_sources) $(glue_sources) $(ui_sources)		\
main.cc cfgfile.cc consts.cc cfg.cc globals.cc cfgfile.h consts.h	\
cfg.h globals.h

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/glue	\
-I$(top_srcdir)/ui -I$(top_srcdir)/libyacurs/include		\
-I$(top_builddir)/libyacurs/include -I$(top_srcdir)/pwgen	\
-I$(top_srcdir) -DLOCALEDIR=\"$(localedir)\"

yapet_LDADD = $(top_builddir)/libyacurs/src/libyacurs.la $(top_builddir)/crypt/libyapet-crypt.la @LIBINTL@

if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
