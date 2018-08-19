noinst_LTLIBRARIES = libyapet-file.la

libyapet_file_la_SOURCES = securearray.hh securearray.cc rawfile.cc rawfile.hh ods.hh ileerror.hh fileerror.cc

libyapet_file_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
libyapet_file_la_CPPFLAGS += -DNDEBUG
else
libyapet_file_la_CPPFLAGS += -DDEBUG
endif