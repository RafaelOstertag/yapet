# $Id$

EXTRA_DIST = Doxyfile.in README.sgml.in INSTALL.sgml.in preamble.sgml html/

doxy_DATA = html/
doxydir = $(docdir)

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

README.sgml: Makefile $(srcdir)/README.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

if HAVEDOCBOOK2TXT
README: Makefile README.sgml
	$(DOCBOOK2TXT) $(srcdir)/README.sgml

INSTALL: Makefile INSTALL.sgml
	$(DOCBOOK2TXT) $(srcdir)/INSTALL.sgml

endif

if HAVEDOXYGEN
$(doxy_DATA): Doxyfile
	$(DOXYGEN)
endif

dist-hook: html/ README INSTALL

maintainer-clean-local:
	rm -r html
