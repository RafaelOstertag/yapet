# $Id$

if !DISABLEINSTALLDOC
htmldoc_DATA = README.html DESIGN.html yapet.html

if BUILDCSV2YAPET
htmldoc_DATA += csv2yapet.html
endif # BUILDCSV2YAPET

htmldocdir = $(docdir)/html

dist_man1_MANS = yapet.1

if BUILDCSV2YAPET
dist_man1_MANS += csv2yapet.1
endif # BUILDCSV2YAPET
endif # !DISABLEINSTALLDOC

sed_files = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in yapet.sgml.in	\
csv2yapet.sgml.in README.Cygwin.sgml.in

sgml_static_files = copyright.sgml supportedplatforms.sgml caution.sgml	\
pwrecord.sgml license.sgml bugreport.sgml

sgml_files = yapet.sgml README.sgml README.Cygwin.sgml INSTALL.sgml DESIGN.sgml	\
$(sgml_static_files)

# The value of _BEGIN and _END variables will be used to replace markers in the
# xml documention with either nothin, or xml comments. This way we can
# include/exclude stuff from the docs without using conditional processing of
# the xml files.
if BUILDCSV2YAPET
sgml_files += csv2yapet.sgml
DOCCSV2YAPET_BEGIN =
DOCCSV2YAPET_END =
else
DOCCSV2YAPET_BEGIN = <!--
DOCCSV2YAPET_END = -->
endif
# Used to conditionally include documentation for NLS support.
#
# Please not that the xml comments have to include a block of text.
if USE_NLS
DOCNLS = 
else
DOCNLS = -e '/<!--[ \t]*NLS_BEGIN[ \t]*-->/,/<!--[ \t]*NLS_END[ \t]*-->/d'
endif
# Used to conditionally include documentation for password generator.
#
# Please not that the xml comments have to include a block of text.
if BUILDPWGEN
DOCPWGEN= 
DOCNOTPWGEN = -e '/<!--[ \t]*NOTPWGEN_BEGIN[ \t]*-->/,/<!--[ \t]*NOTPWGEN_END[ \t]*-->/d'
else
DOCPWGEN = -e '/<!--[ \t]*PWGEN_BEGIN[ \t]*-->/,/<!--[ \t]*PWGEN_END[ \t]*-->/d'
DOCNOTPWGEN =
endif

doxycfg_src = Doxyfile.in

EXTRA_DIST = $(htmldoc_DATA) $(sgml_files) $(sed_static_files) $(sed_files) $(doxycfg_src)

editfile = $(SED) -e 's|@PACKAGE_STRING[@]|$(PACKAGE_STRING)|g' \
	-e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@PACKAGE[@]|$(PACKAGE)|g' \
	-e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g' \
	-e 's|@srcdir[@]|$(srcdir)|g' \
	-e 's|@top_srcdir[@]|$(top_srcdir)|g' \
	-e 's|@top_builddir[@]|$(top_builddir)|g' \
	-e 's|@builddir[@]|$(builddir)|g' \
	-e 's|<!--[ \t]*CSV2YAPET_BEGIN[ \t]*-->|$(DOCCSV2YAPET_BEGIN)|g' \
	-e 's|<!--[ \t]*CSV2YAPET_END[ \t]*-->|$(DOCCSV2YAPET_END)|g' \
	$(DOCPWGEN) $(DOCNOTPWGEN) $(DOCNLS)

if BUILDDOC

# Those files are fed to xsltproc
sgml_html = README.sgml README.Cygwin.sgml INSTALL.sgml DESIGN.sgml yapet.sgml
if BUILDCSV2YAPET
sgml_html +=  csv2yapet.sgml
endif

sgml_text = README.sgml INSTALL.sgml DESIGN.sgml README.Cygwin.sgml
sgml_man = yapet.sgml
if BUILDCSV2YAPET
sgml_man += csv2yapet.sgml
endif

docbook_docbook_xsl = @DOCBOOKXSL@/xhtml/docbook.xsl
docbook_man_xsl = @DOCBOOKXSL@/manpages/docbook.xsl

sed_files_target = $(sed_files:.in=)
sgml_html_targets = $(sgml_html:.sgml=.html)
sgml_text_target = $(sgml_text:.sgml=)

if HAVEXSLTPROC

$(sed_files_target): $(sed_files) Makefile
	$(editfile) $(srcdir)/$@.in > $(builddir)/$@

$(sgml_html_targets): $(sgml_files) $(sgml_html)
	rm -f $(builddir)/$@
	$(XSLTPROC) -o $(builddir)/$@ $(docbook_docbook_xsl) $(builddir)/$(@:.html=.sgml)

$(sgml_text_target): $(sgml_files) $(sgml_text)
	$(XSLTPROC) -o $(builddir)/$@.tmp $(docbook_docbook_xsl) $(builddir)/$@.sgml
	$(LYNX) -force_html -nolist -dump $(builddir)/$@.tmp > $(top_srcdir)/$@
	rm $(builddir)/$@.tmp
	touch $(builddir)/$@

$(dist_man1_MANS): $(sgml_files)
	rm -f $(srcdir)/$@
	$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) $(builddir)/$(@:.1=.sgml)

clean-local:
	rm -f $(sgml_text_target)
	rm -f $(sgml_html_targets)
	rm -f $(sed_files_target)

endif # HAVEXSLTPROC

endif # BUILDDOC

dist-hook: $(sgml_html_targets) $(dist_man1_MANS) $(sgml_text_target)

if HAVEDOXYGEN
Doxyfile: Doxyfile.in
	$(editfile) $(srcdir)/$@.in > $(builddir)/$@

source-doc: Doxyfile
	$(DOXYGEN)

endif # HAVEDOXYGEN

if !DISABLEINSTALLDOC
if BUILDDOC

if BUILDSOURCEDOC
all-local: $(sgml_html_targets) $(sgml_text_target) source-doc $(dist_man1_MANS)
else
all-local: $(sgml_html_targets) $(sgml_text_target) $(dist_man1_MANS)
endif # BUILDSOURCEDOC

else # BUILDDOC

if BUILDSOURCEDOC
all-local: source-doc
endif #BUILDSOURCEDOC

endif # BUILDDOC
endif # !DISABLEINSTALLDOC

maintainer-clean-local:
	rm -rf source-doc
	rm -f $(sgml_html_targets) $(sgml_text_target) $(sed_files_target) $(dist_man1_MANS)
