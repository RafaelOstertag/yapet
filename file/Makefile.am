noinst_LTLIBRARIES = libyapet-file.la

libyapet_file_la_SOURCES = rawfile.cc rawfile.hh \
ods.hh fileerror.hh fileerror.cc fileutils.cc fileutils.hh yapetfile.hh \
yapetfile.cc yapet10file.hh yapet10file.cc

libyapet_file_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir) -I$(top_srcdir)/utils

if DISABLE_DEBUG
libyapet_file_la_CPPFLAGS += -DNDEBUG
else
libyapet_file_la_CPPFLAGS += -DDEBUG
endif