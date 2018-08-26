# $Id$

AM_CPPFLAGS = -I$(top_srcdir)/src/yapet \
    -I$(srcdir)/../crypt		\
    -I$(srcdir)/../pwgen \
    -I$(srcdir)/../ui \
    -I$(srcdir)/../consts \
    -I$(srcdir)/../cfg \
    -I$(top_srcdir)/libyacurs/include \
    -I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-glue.la

libyapet_glue_la_SOURCES = loadfile.cc promptpassword.cc createfile.cc	\
changepassword.cc loadfile.h promptpassword.h createfile.h	\
changepassword.h opencmdlinefile.cc opencmdlinefile.h
