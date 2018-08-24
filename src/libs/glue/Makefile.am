# $Id$

noinst_LTLIBRARIES = libyapet-glue.la

libyapet_glue_la_SOURCES = loadfile.cc promptpassword.cc createfile.cc	\
changepassword.cc loadfile.h promptpassword.h createfile.h	\
changepassword.h opencmdlinefile.cc opencmdlinefile.h

libyapet_glue_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"		\
-I$(top_srcdir) -I$(top_srcdir)/src/yapet -I$(srcdir)/../crypt		\
-I$(top_srcdir)/libyacurs/include -I$(top_builddir)/libyacurs/include \
-I$(srcdir)/../pwgen -I$(srcdir)/../ui

if DISABLE_DEBUG
libyapet_glue_la_CPPFLAGS += -DNDEBUG
else
libyapet_glue_la_CPPFLAGS += -DDEBUG
endif
