
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir) -I$(top_srcdir)/src/libs/utils

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-file.la
libyapet_file_la_SOURCES = rawfile.cc rawfile.hh \
ods.hh fileerror.hh fileerror.cc fileutils.cc fileutils.hh yapetfile.hh \
yapetfile.cc yapet10file.hh yapet10file.cc header10.cc header10.hh headererror.hh headerversion.hh
