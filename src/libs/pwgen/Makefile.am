# $Id$

noinst_LTLIBRARIES = libyapet-pwgen.la

libyapet_pwgen_la_SOURCES = pwgenexception.h charpool.cc charpool.h rng.cc rng.h	\
pwgen.cc pwgen.h

libyapet_pwgen_la_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
libyapet_pwgen_la_CPPFLAGS += -DNDEBUG
else
libyapet_pwgen_la_CPPFLAGS += -DDEBUG
endif