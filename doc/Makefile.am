# $Id$

sgml_files = README.sgml.in INSTALL.sgml.in preamble.sgml

EXTRA_DIST = Doxyfile.in $(sgml_files) html/


editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g'

Doxyfile: Makefile $(srcdir)/Doxyfile.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

INSTALL.sgml: Makefile $(srcdir)/INSTALL.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

if HAVEDOXYGEN
doxy = html/
$(doxy): Doxyfile
	$(DOXYGEN)
endif

dist-hook: $(doxy)

maintainer-clean-local:
	rm -r html
