# $Id$

noinst_LIBRARIES = libglue.a

libglue_a_SOURCES =  promptpassword.cc loadfile.cc
libglue_a_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/libyacurs/include -I$(top_srcdir)/yapet -I$(top_srcdir)/yapet/ui -DLOCALEDIR=\"$(localedir)\"
libglue_a_LIBADD = @LIBINTL@
