# $Id$
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-pwgen.la

libyapet_pwgen_la_SOURCES = pwgenexception.h charpool.cc charpool.h rng.cc rng.h	\
pwgen.cc pwgen.h
