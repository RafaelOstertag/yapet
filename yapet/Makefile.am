# $Id$

bin_PROGRAMS = yapet

pwgen_sources = ../pwgen/charpool.cc		\
../pwgen/rng.cc ../pwgen/pwgen.cc

glue_sources = ../glue/loadfile.cc				\
../glue/promptpassword.cc ../glue/createfile.cc	\
../glue/changepassword.cc					\
../glue/opencmdlinefile.cc

ui_sources = ../ui/help.cc ../ui/mainwindow.cc	\
../ui/passworddialog.cc ../ui/passwordrecord.cc	\
../ui/newpassworddialog.cc ../ui/info.cc		\
../ui/yapetunlockdialog.cc					\
../ui/yapetlockscreen.cc ../ui/pwgendialog.cc

yapet_LDFLAGS = -static

yapet_SOURCES = $(pwgen_sources) $(glue_sources) $(ui_sources)		\
main.cc cfgfile.cc consts.cc cfg.cc globals.cc cfgfile.h consts.h	\
cfg.h globals.h

yapet_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/glue	\
-I$(top_srcdir)/ui -I$(top_srcdir)/libyacurs/include		\
-I$(top_builddir)/libyacurs/include -I$(top_srcdir)/pwgen	\
-I$(top_srcdir) -DLOCALEDIR=\"$(localedir)\" $(OPENSSL_INCLUDES)

yapet_LDADD = $(top_builddir)/libyacurs/src/libyacurs.la $(top_builddir)/crypt/libyapet-crypt.la $(LIBINTL)

if DISABLE_DEBUG
yapet_CPPFLAGS += -DNDEBUG
else
yapet_CPPFLAGS += -DDEBUG
endif

clean-local:
	rm -rf SunWS_cache
