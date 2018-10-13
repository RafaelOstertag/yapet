DISTCHECK_CONFIGURE_FLAGS = --enable-install-doc	\
--enable-silent-rules

EXTRA_DIST = build-aux/config.rpath \
LICENSE INSTALL.generic yapet.desktop.in intl.h gettext.h BUGS

DIST_SUBDIRS = libyacurs src po tests doc

SUBDIRS = libyacurs src po tests doc

desktopfile_DATA = yapet.desktop
desktopfiledir = $(datadir)/applications

if !DISABLEINSTALLDOC
textdoc_DATA = README COPYING AUTHORS LICENSE
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

maintainer-clean-local:
	rm -f README.Cygwin
