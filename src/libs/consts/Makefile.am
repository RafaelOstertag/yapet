AM_CPPFLAGS = -I$(srcdir)/../pwgen \
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-consts.la
libyapet_consts_la_SOURCES = consts.cc consts.h
