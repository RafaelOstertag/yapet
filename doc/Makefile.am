# $Id$

EXTRA_DIST = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in		\
yapet.sgml.in Doxyfile.in preamble.sgml supportedplatforms.sgml	\
caution.sgml README.html INSTALL.html DESIGN.html yapet.html		\
html-doc

sed_files = README.sgml INSTALL.sgml DESIGN.sgml yapet.sgml	\
Doxyfile

sgml_files = yapet.sgml README.sgml INSTALL.sgml DESIGN.sgml	\
preamble.sgml supportedplatforms.sgml caution.sgml		\
manfigures/mainscreen.sgml

# Those files are fed to xsltproc
sgml_main = README.sgml INSTALL.sgml DESIGN.sgml yapet.sgml

man_target = yapet

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

$(sed_files):
	rm -f "$@"
	$(editfile) "$@.in" > "$@"

if HAVEXSLTPROC

html: stamp_html

stamp_html: $(sgml_files)
	for f in $(sgml_main) ; do \
		nosuffix=`echo $$f | $(SED) s/\.sgml//` ; \
		rm -f "$$nosuffix.html" ; \
		test -f "$$f" && $(XSLTPROC) -o "$$nosuffix.html" $(docbook_docbook_xsl) "$$f" ; \
	done
	touch stamp_html

plaintext: stamp_plaintext

stamp_plaintext: $(sgml_files)
	for f in $(sgml_main) ; do \
		nosuffix=`echo $$f | $(SED) s/\.sgml//` ; \
		rm -f "$(top_srcdir)/$$nosuffix" ; \
		test -f "$$f" && $(LYNX) -nolist -dump "$$nosuffix.html" > "$(top_srcdir)/$$nosuffix" ; \
	done
	touch stamp_plaintext

yapet.1: $(YAPETMAN_sgmlfiles)
	rm -f $(srcdir)/yapet.1
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) $(srcdir)/yapetman.sgml

endif # if HAVEXSLTPROC

if HAVEDOXYGEN
html-doc: Doxyfile
	$(DOXYGEN)
else
html-doc: Doxyfile

endif

dist-hook: html-doc html plaintext
all-local: html plaintext

maintainer-clean-local:
	rm -rf html-doc
	rm -f stamp_plaintext
	rm -f stamp_html
