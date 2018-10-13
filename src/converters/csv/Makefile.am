yapet_src = $(top_srcdir)/src
yapet_src_libdir = $(yapet_src)/libs
yapet_libdir = $(top_builddir)/src/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/pwgen \
	-I$(yapet_src_libdir)/consts \
	-I$(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = $(LIBINTL)

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-csv.la
libyapet_csv_la_SOURCES = csvstringfield.hh csvstringfield.cc csvline.hh csvline.cc