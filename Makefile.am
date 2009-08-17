# $Id$

AUTOMAKE_OPTIONS = check-news dist-bzip2
DISTCHECK_CONFIGURE_FLAGS = --enable-csv2yapet --enable-terminal-title --disable-source-doc --enable-install-doc --enable-pwgen --disable-assert

EXTRA_DIST = config.rpath m4/ChangeLog README.Cygwin MAINTAINER DESIGN LICENSE	\
INSTALL.generic Makefile.cvs yapet.desktop.in intl.h gettext.h BUGS

SUBDIRS = intl po crypt ui yapet

if BUILDCSV2YAPET
SUBDIRS += csv2yapet
endif
SUBDIRS += tests doc

desktopfile_DATA = yapet.desktop
desktopfiledir = $(datadir)/applications

if !DISABLEINSTALLDOC
textdoc_DATA = README COPYING AUTHORS DESIGN LICENSE
textdocdir = $(docdir)
endif # !DISABLEINSTALLDOC

editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g' \
	-e 's|@PACKAGE[@]|$(PACKAGE)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g' \
	-e 's|@prefix[@]|$(prefix)|g'

yapet.desktop: Makefile $(srcdir)/yapet.desktop.in
	rm -f $@.tmp $@
	$(editfile) $(srcdir)/$@.in > $@

DISTCLEANFILES = yapet.desktop

ACLOCAL_AMFLAGS = -I m4

if !DISABLE_ASSERT
all-local:
	@echo ""
	@echo "***************************************************"
	@echo "*                                                 *"
	@echo "* Please note: asserts are enabled in this build, *"
	@echo "*              it should therefore not be used    *"
	@echo "*              for stable distributions.          *"
	@echo "*                                                 *"
	@echo "* You  can  disable  asserts  by  providing       *"
	@echo "* --disable-assert when invoking configure.       *"
	@echo "*                                                 *"
	@echo "***************************************************"
	@echo ""
endif

clean-local:
	rm -rf SunWS_cache

maintainer-clean-local:
	rm -f README.Cygwin
