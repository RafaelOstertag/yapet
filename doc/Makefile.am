# $Id$

htmldoc_DATA = README.html DESIGN.html yapet.html
htmldocdir = $(docdir)/html

dist_man1_MANS = yapet.1

sed_files = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in	\
yapet.sgml.in Doxyfile.in

sgml_static_files = preamble.sgml supportedplatforms.sgml caution.sgml	\
pwrecord.sgml

sgml_files = yapet.sgml README.sgml INSTALL.sgml DESIGN.sgml	\
$(sgml_static_files)

EXTRA_DIST = source-doc/ $(htmldoc_DATA) $(sgml_files) $(sed_static_files)

if BUILDDOC

# Those files are fed to xsltproc
sgml_html = README.sgml INSTALL.sgml DESIGN.sgml yapet.sgml
sgml_text = README.sgml INSTALL.sgml DESIGN.sgml
sgml_man = yapet.sgml

docbook_docbook_xsl = http://docbook.sourceforge.net/release/xsl/current/xhtml/docbook.xsl
docbook_man_xsl = http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl

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

if HAVEXSLTPROC

$(sed_files_target): $(sed_files)
	$(editfile) "$(srcdir)/$@.in" > "$@"

$(sgml_html_targets): $(sgml_files) $(sgml_html)
	nosuffix=`echo "$@" | $(SED) s/\.html//` ; \
	rm -f "$@" ; \
	$(XSLTPROC) -o "$@" $(docbook_docbook_xsl) "$$nosuffix.sgml"

$(sgml_text_target): $(sgml_files) $(sgml_text)
	$(XSLTPROC) -o "$@.tmp" $(docbook_docbook_xsl) "$@.sgml"
	$(LYNX) -force_html -nolist -dump "$@.tmp" > "$@"
	rm "$@.tmp"
	mv -f "$@" "$(top_srcdir)/$@"
	touch $@

yapet.1: $(sgml_files)
	rm -f $@
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) yapet.sgml

clean-local:
	rm -f $(sgml_text_target)

else
yapet.1:
	@echo "You don't have xsltproc installed."
	@echo "Unable to build man page"

$(sed_files_target):
	@echo "You don't have xsltproc installed."

$(sgml_html_targets):
	@echo "You don't have xsltproc installed."
	@echo "Unable to build html pages"

$(sgml_text_target):
	@echo "You don't have xsltproc installed."
	@echo "Unable to build text files"

endif # HAVEXSLTPROC

if HAVEDOXYGEN
source-doc: Doxyfile
	$(DOXYGEN)
else
source-doc:
	@echo "You don't have doxygen installed."
	@echo "Unable to build source code documentation"

endif # HAVEDOXYGENT

endif # BUILDDOC

dist-hook: source-doc $(sgml_html_targets) yapet.1


if BUILDDOC
all-local: $(sgml_html_targets) $(sgml_text_target) source-doc yapet.1
endif

maintainer-clean-local:
	rm -rf source-doc
	rm -f $(sgml_html_targets) $(sgml_text_target) $(sed_files_target) yapet.1
