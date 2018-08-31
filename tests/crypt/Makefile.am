yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

# Tests may fail if not run serial
AUTOMAKE_OPTIONS = serial-tests

# For silent rules
cpy_verbose = $(cpy_verbose_$(V))
cpy_verbose_ = $(cpy_verbose_$(AM_DEFAULT_VERBOSITY))
cpy_verbose_0 = @echo "  CPY    $@";
# For silent rules
chmod_verbose = $(chmod_verbose_$(V))
chmod_verbose_ = $(chmod_verbose_$(AM_DEFAULT_VERBOSITY))
chmod_verbose_0 = @echo "  CHMOD  $@";

CLEANFILES = testfile.gps encryptiontest.gps encryptiontest.gps.bak	\
test1.pet test2.pet test3.pet test4.pet test5.pet test6.pet test7.pet	\
test8.pet test9.pet test10.pet corrupt.pet f32le0.5.pet	\
f32be0.5.pet f64le0.5.pet f64be0.5.pet f32le0.6.pet f32be0.6.pet	\
f64le0.6.pet f64be0.6.pet

#
# Files used for foreign test
#
# f32le0.5.pet.in	file created on 32bit little endian arch with a pre 0.6 version
# f32be0.5.pet.in	file created on 32bit big endian arch with a pre 0.6 version
# f64le0.5.pet.in	file created on 64bit little endian arch with a pre 0.6 version
# f64be0.5.pet.in	file created on 64bit big endian arch with a pre 0.6 version
#
# f32le0.6.pet.in	file created on 32bit little endian arch with a 0.6 version
# f32be0.6.pet.in	file created on 32bit big endian arch with a 0.6 version
# f64le0.6.pet.in	file created on 64bit little endian arch with a 0.6 version
# f64be0.6.pet.in	file created on 64bit big endian arch with a 0.6 version
EXTRA_DIST = corrupt.pet.in f32le0.5.pet.in		\
f32be0.5.pet.in f64le0.5.pet.in f64be0.5.pet.in f32le0.6.pet.in		\
f32be0.6.pet.in f64le0.6.pet.in f64be0.6.pet.in testpaths.h.in

# Listed as _DEPENDENCIES for corrupt and foreign
.pet.in.pet:
	$(cpy_verbose)cp $< $(builddir)/$@
	$(chmod_verbose)chmod u=rw $(builddir)/$@

check_PROGRAMS = key record bdbuffer enc partdec file1 file2	\
file3 file4 file5 file6 corrupt foreign

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/file \
	-I$(yapet_libs_srcdir)/utils \
    -I$(builddir)/.. \
    -I$(srcdir)/.. \
	-I$(top_srcdir)
LDADD = $(LIBINTL)
AM_LDFLAGS = $(yapet_libs_builddir)/crypt/libyapet-crypt.la  \
	$(yapet_libs_builddir)/file/libyapet-file.la \
	$(yapet_libs_builddir)/utils/libyapet-utils.la

corrupt_SOURCES = corrupt.cc
corrupt_DEPENDENCIES = corrupt.pet

foreign_SOURCES = foreign.cc
foreign_DEPENDENCIES = f32le0.5.pet f32be0.5.pet f64le0.5.pet f64be0.5.pet	\
f32le0.6.pet f32be0.6.pet f64le0.6.pet f64be0.6.pet

key_SOURCES = key.cc

record_SOURCES = record.cc

bdbuffer_SOURCES = bdbuffer.cc

enc_SOURCES = enc.cc

partdec_SOURCES = partdec.cc

file1_SOURCES = file1.cc

file2_SOURCES = file2.cc

file3_SOURCES = file3.cc

file4_SOURCES = file4.cc

file5_SOURCES = file5.cc

file6_SOURCES = file6.cc

SUFFIXES = .pet .pet.in