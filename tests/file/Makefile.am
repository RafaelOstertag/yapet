yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

# For silent rules
cpy_verbose = $(cpy_verbose_$(V))
cpy_verbose_ = $(cpy_verbose_$(AM_DEFAULT_VERBOSITY))
cpy_verbose_0 = @echo "  CPY    $@";
# For silent rules
chmod_verbose = $(chmod_verbose_$(V))
chmod_verbose_ = $(chmod_verbose_$(AM_DEFAULT_VERBOSITY))
chmod_verbose_0 = @echo "  CHMOD  $@";

EXTRA_DIST = testpaths.h.in yapet10file-corrupt-identifier.pet.in yapet20file-corrupt-identifier.pet.in
CLEANFILES = yapet-fileutils-test yapet-rawfile-test yapet-yapet10file-test yapet-yapet20file-test \
 yapet-yapetfile-test yapet10file-corrupt-identifier.pet yapet20file-corrupt-identifier.pet yape-filehelper-test

check_PROGRAMS = rawfile fileutils yapetfile yapet10file yapet20file header10 headerversion filehelper
TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/pwgen \
    -I$(yapet_libs_srcdir)/consts \
    -I$(yapet_libs_srcdir)/metadata \
    -I$(yapet_libs_srcdir)/exceptions \
    -I$(yapet_libs_srcdir)/utils \
    -I$(yapet_libs_srcdir)/interfaces \
    -I$(yapet_libs_srcdir)/file \
    -I$(top_srcdir)
LDADD = $(yapet_libs_builddir)/file/libyapet-file.la \
    $(yapet_libs_builddir)/utils/libyapet-utils.la \
    $(yapet_libs_builddir)/libyapet-logger.la \
    $(yapet_libs_builddir)/metadata/libyapet-metadata.la
AM_LDFLAGS =  $(CPPUNIT_LIBS) $(LIBINTL)
AM_CXXFLAGS =  $(CPPUNIT_CFLAGS)

# Listed as _DEPENDENCIES for corrupt and foreign
.pet.in.pet:
	$(cpy_verbose)cp $< $(builddir)/$@
	$(chmod_verbose)chmod u=rw $(builddir)/$@

rawfile_SOURCES = rawfile.cc
fileutils_SOURCES = fileutils.cc
yapetfile_SOURCES = yapetfile.cc
yapet10file_SOURCES = yapet10file.cc
yapet10file_DEPENDENCIES = yapet10file-corrupt-identifier.pet
yapet20file_SOURCES = yapet20file.cc
yapet20file_DEPENDENCIES = yapet20file-corrupt-identifier.pet
header10_SOURCES = header10.cc
headerversion_SOURCES = headerversion.cc
filehelper_SOURCES = filehelper.cc

SUFFIXES = .pet .pet.in
