# $Id$

htmldoc_DATA = README.html DESIGN.html yapet.html csv2yapet.html
htmldocdir = $(docdir)/html

dist_man1_MANS = yapet.1 csv2yapet.1

sed_files = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in yapet.sgml.in	\
csv2yapet.sgml.in Doxyfile.in README.Cygwin.sgml.in

sgml_static_files = copyright.sgml supportedplatforms.sgml caution.sgml	\
pwrecord.sgml license.sgml

sgml_files = yapet.sgml csv2yapet.sgml README.sgml README.Cygwin.sgml	\
INSTALL.sgml DESIGN.sgml $(sgml_static_files)

EXTRA_DIST = source-doc/ $(htmldoc_DATA) $(sgml_files) $(sed_static_files) $(sed_files)

if BUILDDOC

if BUILDCSV2YAPET
xsltprocprofile = --stringparam profile.condition csv2yapet
endif

# Those files are fed to xsltproc
sgml_html = README.sgml INSTALL.sgml DESIGN.sgml yapet.sgml csv2yapet.sgml
sgml_text = README.sgml INSTALL.sgml DESIGN.sgml README.Cygwin.sgml
sgml_man = yapet.sgml csv2yapet.sgml

docbook_docbook_xsl = @DOCBOOKXSL@/xhtml/docbook.xsl
docbook_man_xsl = @DOCBOOKXSL@/manpages/docbook.xsl

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
	rm -f "$@"
	$(XSLTPROC) -o "$@" $(xsltprocprofile) $(docbook_docbook_xsl) "$(@:.html=.sgml)"

$(sgml_text_target): $(sgml_files) $(sgml_text)
	$(XSLTPROC) -o "$@.tmp"  $(xsltprocprofile) $(docbook_docbook_xsl) "$@.sgml"
	$(LYNX) -force_html -nolist -dump "$@.tmp" > "$@"
	rm "$@.tmp"
	mv -f "$@" "$(top_srcdir)/$@"
	touch $@

$(dist_man1_MANS): $(sgml_files)
	rm -f $@
	$(XSLTPROC) -o $(srcdir)/$@  $(xsltprocprofile) $(docbook_man_xsl) $(@:.1=.sgml)

clean-local:
	rm -f $(sgml_text_target)
	rm -f $(sgml_html_targets)
	rm -f $(sed_files_target)

else
$(dist_man1_MANS):
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

dist-hook: source-doc $(sgml_html_targets) $(dist_man1_MANS)


if BUILDDOC
all-local: $(sgml_html_targets) $(sgml_text_target) source-doc $(dist_man1_MANS)
endif

maintainer-clean-local:
	rm -rf source-doc
	rm -f $(sgml_html_targets) $(sgml_text_target) $(sed_files_target) $(dist_man1_MANS)
