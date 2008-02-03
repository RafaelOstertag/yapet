# $Id$

bin_PROGRAMS = gpsafe

noinst_HEADERS = mainwindow.h statusbar.h fileopen.h passworddialog.h	\
passwordrecord.h

gpsafe_SOURCES = main.cc mainwindow.cc statusbar.cc fileopen.cc	\
passworddialog.cc passwordrecord.cc

gpsafe_CXXFLAGS = -I$(top_srcdir)/crypt -I$(top_srcdir)/ui
gpsafe_LDADD = $(top_builddir)/crypt/libgpcrypt.a $(top_builddir)/ui/libui.a
