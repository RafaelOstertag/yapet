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
sgml_html_targets = $(sgml_html:.sgml=.html)
sgml_text_target = $(sgml_text:.sgml=)

$(sed_files_target): $(sed_files)
	$(editfile) "$(srcdir)/$@.in" > "$@"

if HAVEXSLTPROC

$(sgml_html_targets): $(sgml_files) $(sgml_html)
	nosuffix=`echo "$@" | $(SED) s/\.html//` ; \
	rm -f "$@" ; \
	$(XSLTPROC) -o "$@" $(docbook_docbook_xsl) "$$nosuffix.sgml"

$(sgml_text_target): $(sgml_files) $(sgml_text)
	$(XSLTPROC) -o "$@.tmp" $(docbook_docbook_xsl) "$@.sgml"
	$(LYNX) -nolist -dump "$@.tmp" > "$@"
	rm "$@.tmp"
	cp "$@" "$(top_srcdir)/$@

$(srcdir)/yapet.1: $(sgml_files)
	rm -f $@
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) yapet.sgml

else

$(htmldoc_DATA):

$(sgml_html_targets):

$(sgml_text_target):

$(srcdir)/yapet.1:

endif # if HAVEXSLTPROC

if HAVEDOXYGEN
source-doc: Doxyfile
	$(DOXYGEN)
else
source-doc: Doxyfile

endif

dist-hook: source-doc $(sgml_html_targets) $(sgml_text_target) $(srcdir)/yapet.1
	cp -r $(srcdir)/source-doc $(distdir)

distclean-local:
	rm -f $(sed_files_target)

all-local: $(sgml_html_targets) $(sgml_text_target) source-doc $(srcdir)/yapet.1

maintainer-clean-local:
	rm -rf source-doc
	rm -f $(sgml_html_targets) $(sgml_text_target)
