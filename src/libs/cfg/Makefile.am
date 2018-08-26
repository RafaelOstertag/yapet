AM_CPPFLAGS = -I$(srcdir)/../pwgen \
	-I$(srcdir)/../consts \
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-cfg.la
libyapet_cfg_la_SOURCES = cfg.cc cfg.h cfgfile.cc cfgfile.h