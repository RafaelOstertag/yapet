yapet_srcdir = $(top_srcdir)/src
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src
yapet_libs_builddir = $(top_builddir)/src/libs

# For silent rules
cpy_verbose = $(cpy_verbose_$(V))
cpy_verbose_ = $(cpy_verbose_$(AM_DEFAULT_VERBOSITY))
cpy_verbose_0 = @echo "  CPY    $@";
# For silent rules
chmod_verbose = $(chmod_verbose_$(V))
chmod_verbose_ = $(chmod_verbose_$(AM_DEFAULT_VERBOSITY))
chmod_verbose_0 = @echo "  CHMOD  $@";

CLEANFILES = testfile_blowfish.gps testfile_blowfish.gps.bak encryptiontest.gps encryptiontest.gps.bak	\
test1.pet test2.pet test3.pet test4.pet test5.pet test6.pet test7.pet	\
test8.pet test9.pet test10.pet corrupt_blowfish.pet corrupt_aes256.pet f32le0.5.pet	\
f32be0.5.pet f64le0.5.pet f64be0.5.pet f32le0.6.pet f32be0.6.pet	\
f64le0.6.pet f64be0.6.pet cryptofactoryhelper-1.0.pet cryptofactoryhelper-2.0.pet \
cryptofactoryhelper-tooshort.pet cryptofactoryhelper-unknown.pet \
testfile_aes256.gps.bak testfile_aes256.gps

# We have to copy the files under test to the build dir and adjust the permission
# to read/write. This is necessary when running distcheck, which makes the source
# directories read-only. Tests may fail when they try to open these files 
# read/write
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
EXTRA_DIST = corrupt_blowfish.pet.in corrupt_aes256.pet.in f32le0.5.pet.in		\
f32be0.5.pet.in f64le0.5.pet.in f64be0.5.pet.in f32le0.6.pet.in		\
f32be0.6.pet.in f64le0.6.pet.in f64be0.6.pet.in testpaths.h.in \
cryptofactoryhelper-1.0.pet.in cryptofactoryhelper-2.0.pet.in \
cryptofactoryhelper-unknown.pet.in cryptofactoryhelper-tooshort.pet.in

# Listed as _DEPENDENCIES for corrupt and foreign
.pet.in.pet:
	$(cpy_verbose)cp $< $(builddir)/$@
	$(chmod_verbose)chmod u=rw $(builddir)/$@

check_PROGRAMS = key448 key256 blowfish aes256 blowfishfactory aes256factory file_blowfish file_aes256 foreign cryptofactoryhelper

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS = -I$(yapet_libs_srcdir)/consts \
	-I$(yapet_libs_srcdir)/exceptions \
	-I$(yapet_libs_srcdir)/metadata \
	-I$(yapet_libs_srcdir)/crypt \
	-I$(yapet_libs_srcdir)/passwordrecord \
	-I$(yapet_libs_srcdir)/interfaces \
	-I$(yapet_libs_srcdir)/pwgen \
	-I$(yapet_libs_srcdir)/file \
	-I$(yapet_libs_srcdir)/utils \
    -I$(builddir)/.. \
    -I$(srcdir)/.. \
	-I$(top_srcdir) \
	$(CPPUNIT_CFLAGS)
LDADD = $(LIBINTL)
AM_LDFLAGS = $(yapet_libs_builddir)/crypt/libyapet-crypt.la  \
	$(yapet_libs_builddir)/file/libyapet-file.la \
	$(yapet_libs_builddir)/utils/libyapet-utils.la \
	$(yapet_libs_builddir)/passwordrecord/libyapet-passwordrecord.la \
	$(yapet_libs_builddir)/metadata/libyapet-metadata.la \
	$(CPPUNIT_LIBS)

foreign_SOURCES = foreign.cc
foreign_DEPENDENCIES = f32le0.5.pet f32be0.5.pet f64le0.5.pet f64be0.5.pet	\
f32le0.6.pet f32be0.6.pet f64le0.6.pet f64be0.6.pet

key448_SOURCES = key448.cc
key256_SOURCES = key256.cc
blowfish_SOURCES = blowfish.cc
aes256_SOURCES = aes256.cc
blowfishfactory_SOURCES = blowfishfactory.cc
aes256factory_SOURCES = aes256factory.cc

file_blowfish_SOURCES = file_blowfish.cc
file_blowfish_DEPENDENCIES = corrupt_blowfish.pet

file_aes256_SOURCES = file_aes256.cc
file_aes256_DEPENDENCIES = corrupt_aes256.pet

cryptofactoryhelper_SOURCES = cryptofactoryhelper.cc
cryptofactoryhelper_DEPENDENCIES = cryptofactoryhelper-1.0.pet cryptofactoryhelper-2.0.pet \
cryptofactoryhelper-unknown.pet cryptofactoryhelper-tooshort.pet

SUFFIXES = .pet .pet.in