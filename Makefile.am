# $Id$

EXTRA_DIST = MAINTAINER DESIGN INSTALL.generic Makefile.cvs	\
yapet.desktop.in

SUBDIRS = crypt ui yapet tests doc

desktopfile_DATA = yapet.desktop
desktopfiledir = $(datadir)/applications

textdoc_DATA = README COPYING AUTHORS DESIGN
textdocdir = $(docdir)

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
