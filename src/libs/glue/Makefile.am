# $Id$

noinst_LTLIBRARIES = libyapet-glue.la

libyapet_glue_la_SOURCES = loadfile.cc promptpassword.cc createfile.cc	\
changepassword.cc loadfile.h promptpassword.h createfile.h	\
changepassword.h opencmdlinefile.cc opencmdlinefile.h

libyapet_glue_la_CPPFLAGS = -I$(top_srcdir)/src/yapet \
    -I$(srcdir)/../crypt		\
    -I$(srcdir)/../pwgen \
    -I$(srcdir)/../ui \
    -I$(srcdir)/../consts \
    -I$(top_srcdir)/libyacurs/include \
    -I$(top_builddir)/libyacurs/include \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
libyapet_glue_la_CPPFLAGS += -DNDEBUG
else
libyapet_glue_la_CPPFLAGS += -DDEBUG
endif
