noinst_LTLIBRARIES = libyapet-utils.la

libyapet_utils_la_SOURCES = securearray.hh securearray.cc

libyapet_utils_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
libyapet_utils_la_CPPFLAGS += -DNDEBUG
else
libyapet_utils_la_CPPFLAGS += -DDEBUG
endif