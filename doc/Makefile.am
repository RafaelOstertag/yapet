# $Id$
EXTRA_DIST = Doxyfile.in README.sgml.in INSTALL.sgml.in preamble.sgml html-doc

README_sgmlfiles = README.sgml preamble.sgml
README_target = $(top_srcdir)/README

INSTALL_sgmlfiles = INSTALL.sgml preamble.sgml
INSTALL_target = $(top_srcdir)/INSTALL

editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g'

Doxyfile: $(srcdir)/Doxyfile.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

INSTALL.sgml: $(srcdir)/INSTALL.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

README.sgml: $(srcdir)/README.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

if HAVEXSLTPROC
$(README_target): $(README_sgmlfiles)
	rm -f $(README_target)
	$(XSLTPROC) -o $(srcdir)/README.html $(XSLFILE) $(srcdir)/README.sgml
	$(LYNX) -nolist -dump $(srcdir)/README.html > $@
	rm -f $(srcdir)/README.html

$(INSTALL_target): $(INSTALL_sgmlfiles)
	rm -f $(INSTALL_target)
	$(XSLTPROC) -o $(srcdir)/INSTALL.html $(XSLFILE) $(srcdir)/INSTALL.sgml
	$(LYNX) -nolist -dump $(srcdir)/INSTALL.html > $@
	rm -f $(srcdir)/INSTALL.html
else
$(README_target): $(README_sgmlfiles)

$(INSTALL_target): $(INSTALL_sgmlfiles)

endif # if HAVEXSLTPROC


if HAVEDOXYGEN
html-doc: Doxyfile
	$(DOXYGEN)
else
html-doc: Doxyfile

endif

dist-hook: html-doc $(INSTALL_target) $(README_target)
all-local: $(INSTALL_target) $(README_target)

maintainer-clean-local:
	rm -rf html-doc
	rm -f README.sgml
	rm -f INSTALL.sgml
