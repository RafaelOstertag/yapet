noinst_LTLIBRARIES = libyapet-consts.la

libyapet_consts_la_SOURCES = consts.cc consts.h

libyapet_consts_la_CPPFLAGS = -I$(srcdir)/../pwgen \
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
libyapet_consts_la_CPPFLAGS += -DNDEBUG
else
libyapet_consts_la_CPPFLAGS += -DDEBUG
endif