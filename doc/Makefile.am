# $Id$

EXTRA_DIST = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in yapet.sgml.in	\
Doxyfile.in preamble.sgml supportedplatforms.sgml caution.sgml pwrecord.sgml	\
README.html DESIGN.html yapet.html

htmldoc_DATA = README.html DESIGN.html yapet.html
htmldocdir = $(docdir)/html

sed_files = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in yapet.sgml.in	\
Doxyfile.in

sgml_files = yapet.sgml README.sgml INSTALL.sgml DESIGN.sgml preamble.sgml	\
supportedplatforms.sgml caution.sgml pwrecord.sgml

# Those files are fed to xsltproc
sgml_html = README.sgml INSTALL.sgml DESIGN.sgml yapet.sgml
sgml_text = README.sgml INSTALL.sgml DESIGN.sgml
sgml_man = yapet.sgml

dist_man1_MANS = yapet.1

docbook_docbook_xsl = $(XSLPATH)/xhtml/docbook.xsl
docbook_man_xsl = $(XSLPATH)/manpages/docbook.xsl


editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE[@]|$(PACKAGE)|g' \
	-e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g' \
	-e 's|@srcdir[@]|$(srcdir)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g'

sed_files_target = $(sed_files:.in=)

$(sed_files_target): $(sed_files)
	$(editfile) "$(srcdir)/$@.in" > "$@"

if HAVEXSLTPROC

$(htmldoc_DATA): stamp-html

htmlfiles: stamp-html

stamp-html: $(sgml_files)
	for f in $(sgml_html) ; do \
		nosuffix=`echo $$f | $(SED) s/\.sgml//` ; \
		rm -f "$$nosuffix.html" ; \
		test -f "$$f" && $(XSLTPROC) -o "$$nosuffix.html" $(docbook_docbook_xsl) "$$f" ; \
	done
	touch stamp-html

plaintext: stamp-plaintext

stamp-plaintext: $(sgml_files)
	for f in $(sgml_text) ; do \
		nosuffix=`echo $$f | $(SED) s/\.sgml//` ; \
		rm -f "$(top_srcdir)/$$nosuffix" ; \
		test -f "$$f" && $(LYNX) -nolist -dump "$$nosuffix.html" > "$(top_srcdir)/$$nosuffix" ; \
	done
	touch stamp-plaintext

$(srcdir)/yapet.1: $(sgml_files)
	rm -f $@
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) yapet.sgml

else

$(htmldoc_DATA):

htmlfiles:

plaintext:

$(srcdir)/yapet.1:

endif # if HAVEXSLTPROC

if HAVEDOXYGEN
source-doc: Doxyfile
	$(DOXYGEN)
else
source-doc: Doxyfile

endif

dist-hook: source-doc htmlfiles plaintext $(srcdir)/yapet.1
	cp -r $(srcdir)/source-doc $(distdir)

all-local: htmlfiles plaintext $(srcdir)/yapet.1
clean-local:
	rm -f stamp-plaintext
	rm -f stamp-html

maintainer-clean-local:
	rm -rf source-doc
	rm -f stamp-plaintext
	rm -f stamp-html
