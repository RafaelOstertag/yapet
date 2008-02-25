# $Id$

EXTRA_DIST = Doxyfile.in README.sgml.in INSTALL.sgml.in DESIGN.sgml.in	\
yapetman.sgml.in preamble.sgml supportedplatforms.sgml caution.sgml	\
html-doc

README_sgmlfiles = README.sgml preamble.sgml supportedplatforms.sgml	\
caution.sgml
README_target = $(top_srcdir)/README

INSTALL_sgmlfiles = INSTALL.sgml preamble.sgml
INSTALL_target = $(top_srcdir)/INSTALL

DESIGN_sgmlfiles = DESIGN.sgml preamble.sgml
DESIGN_target = $(top_srcdir)/DESIGN

YAPETMAN_sgmlfiles = yapetman.sgml caution.sgml manfigures/mainscreen.sgml

dist_man_MANS = yapet.1
man1_MANS = yapet.1

docbook_docbook_xsl = $(XSLPATH)/xhtml/docbook.xsl
docbook_man_xsl = $(XSLPATH)/manpages/docbook.xsl

editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE[@]|$(PACKAGE)|g' \
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

DESIGN.sgml: $(srcdir)/DESIGN.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

yapetman.sgml: $(srcdir)/yapetman.sgml.in
	rm -f $@
	$(editfile) $(srcdir)/$@.in > $@

if HAVEXSLTPROC
$(README_target): $(README_sgmlfiles)
	rm -f $(README_target)
	$(XSLTPROC) -o $(srcdir)/README.html $(docbook_docbook_xsl) $(srcdir)/README.sgml
	$(LYNX) -nolist -dump $(srcdir)/README.html > $@


$(INSTALL_target): $(INSTALL_sgmlfiles)
	rm -f $(INSTALL_target)
	$(XSLTPROC) -o $(srcdir)/INSTALL.html $(docbook_docbook_xsl) $(srcdir)/INSTALL.sgml
	$(LYNX) -nolist -dump $(srcdir)/INSTALL.html > $@


$(DESIGN_target): $(DESIGN_sgmlfiles)
	rm -f $(DESIGN_target)
	$(XSLTPROC) -o $(srcdir)/DESIGN.html $(docbook_docbook_xsl) $(srcdir)/DESIGN.sgml
	$(LYNX) -nolist -dump $(srcdir)/DESIGN.html > $@

yapet.1: $(YAPETMAN_sgmlfiles)
	rm -f $(srcdir)/yapet.1
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) $(srcdir)/yapetman.sgml

else
$(README_target): $(README_sgmlfiles)

$(INSTALL_target): $(INSTALL_sgmlfiles)

$(DESIGN_target): $(DESIGN_sgmlfiles)

yapet.1: $(YAPETMAN_sgmlfiles)

endif # if HAVEXSLTPROC

if HAVEDOXYGEN
html-doc: Doxyfile
	$(DOXYGEN)
else
html-doc: Doxyfile

endif

dist-hook: html-doc $(INSTALL_target) $(README_target) $(DESIGN_target)
all-local: $(INSTALL_target) $(README_target) $(DESIGN_target)

maintainer-clean-local:
	rm -rf html-doc
	rm -f $(README_target) README.sgml README.html
	rm -f $(INSTALL_target) INSTALL.sgml INSTALL.html
	rm -f $(DESIGN_target) DESIGN.sgml DESIGN.html
