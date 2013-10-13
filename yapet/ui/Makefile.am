# $Id$

noinst_LIBRARIES = libui.a

libui_a_SOURCES =  mainwindow.cc help.cc passworddialog.cc passwordrecord.cc
libui_a_CPPFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/libyacurs/include -I$(top_srcdir)/yapet -I$(top_srcdir)/yapet/glue -DLOCALEDIR=\"$(localedir)\"
libui_a_LIBADD = @LIBINTL@
