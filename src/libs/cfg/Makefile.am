noinst_LTLIBRARIES = libyapet-cfg.la

libyapet_cfg_la_SOURCES = cfg.cc cfg.h cfgfile.cc cfgfile.h

libyapet_cfg_la_CPPFLAGS = -I$(srcdir)/../pwgen \
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
libyapet_cfg_la_CPPFLAGS += -DNDEBUG
else
libyapet_cfg_la_CPPFLAGS += -DDEBUG
endif