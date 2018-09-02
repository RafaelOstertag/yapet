yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

check_PROGRAMS = passwordrecord passwordlistitem

TESTS = $(check_PROGRAMS)       

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/passwordrecord \
    -I$(yapet_libs_srcdir)/utils \
    -I$(yapet_libs_srcdir)/interfaces \
    -I$(top_srcdir) 
LDADD = $(yapet_libs_builddir)/utils/libyapet-utils.la \
    $(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la
AM_LDFLAGS =  $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

passwordrecord_SOURCES = passwordrecord.cc
passwordlistitem_SOURCES = passwordlistitem.cc