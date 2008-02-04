# $Id$

sgml_files = README.sgml.in INSTALL.sgml.in preamble.sgml.in

EXTRA_DIST = Doxyfile.in html/ $(sgml_files)


editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g'



Doxyfile: Makefile $(top_srcdir)/doc/Doxyfile.in
	rm -f $@
	$(editfile) $(top_srcdir)/doc/$@.in > $@

if HAVEDOXYGEN
doxy = html/
$(doxy): Doxyfile
	$(DOXYGEN)
endif

dist-hook: $(doxy)

maintainer-clean-local:
	rm -r html
