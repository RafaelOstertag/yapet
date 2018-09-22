yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

check_PROGRAMS = metadata

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/metadata \ 
    -I$(yapet_libs_srcdir)/utils \
    -I$(yapet_libs_srcdir)/interfaces \
    -I$(top_srcdir)
LDADD = $(yapet_libs_builddir)/metadata/libyapet-metadata.la\
    $(yapet_libs_builddir)/utils/libyapet-utils.la
    
AM_LDFLAGS =  $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

metadata_SOURCES = metadata.cc
