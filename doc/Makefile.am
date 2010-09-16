# $Id$

# For fixing bug #22
dist_man1_MANS = yapet.1
if BUILDCSV2YAPET
dist_man1_MANS += csv2yapet.1
endif # BUILDCSV2YAPET

if !DISABLEINSTALLDOC
htmldoc_DATA = README.html DESIGN.html yapet.html

if BUILDCSV2YAPET
htmldoc_DATA += csv2yapet.html
endif # BUILDCSV2YAPET

htmldocdir = $(docdir)/html

endif # !DISABLEINSTALLDOC

# Needed for EXTRA_DIST
sed_files = README.sgml.in INSTALL.sgml.in DESIGN.sgml.in yapet.sgml.in	\
 csv2yapet.sgml.in README.Cygwin.sgml.in

sgml_static_files = copyright.sgml supportedplatforms.sgml caution.sgml	\
pwrecord.sgml license.sgml bugreport.sgml

generated_sgml_files = yapet.sgml csv2yapet.sgml README.sgml README.Cygwin.sgml	\
INSTALL.sgml DESIGN.sgml

# The value of _BEGIN and _END variables will be used to replace
# markers in the xml documentation with either nothing, or xml
# comments. This way we can include/exclude stuff from the docs
# without using conditional processing of the xml files.
if BUILDCSV2YAPET
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

EXTRA_DIST = $(htmldoc_DATA) $(generated_sgml_files) $(sgml_static_files) $(sed_files) $(doxycfg_src)

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

sgml_text = README.sgml INSTALL.sgml DESIGN.sgml README.Cygwin.sgml

docbook_docbook_xsl = @DOCBOOKXSL@/xhtml/docbook.xsl
docbook_man_xsl = @DOCBOOKXSL@/manpages/docbook.xsl

sgml_text_target = $(sgml_text:.sgml=)

if HAVEXSLTPROC

# For silent rules
sed_verbose = $(sed_verbose_$(V))
sed_verbose_ = $(sed_verbose_$(AM_DEFAULT_VERBOSITY))
sed_verbose_0 = @echo "  SED    $<";
# For silent rules
rm_verbose = $(rm_verbose_$(V))
rm_verbose_ = $(rm_verbose_$(AM_DEFAULT_VERBOSITY))
rm_verbose_0 = @echo "  RM     $@";
# For silent rules
xsl_verbose = $(xsl_verbose_$(V))
xsl_verbose_ = $(xsl_verbose_$(AM_DEFAULT_VERBOSITY))
xsl_verbose_0 = @echo "  XSLT   $@";
# For silent rules
touch_verbose = $(touch_verbose_$(V))
touch_verbose_ = $(touch_verbose_$(AM_DEFAULT_VERBOSITY))
touch_verbose_0 = @echo "  TOUCH  $@";
# For silent rules
lynx_verbose = $(lynx_verbose_$(V))
lynx_verbose_ = $(lynx_verbose_$(AM_DEFAULT_VERBOSITY))
lynx_verbose_0 = @echo "  LYNX   $@";
# For silent rules
w3m_verbose = $(w3m_verbose_$(V))
w3m_verbose_ = $(w3m_verbose_$(AM_DEFAULT_VERBOSITY))
w3m_verbose_0 = @echo "  W3M    $@";

.sgml.in.sgml: Makefile
	$(sed_verbose)$(editfile) $< > $(builddir)/$@

.sgml.html: $(generated_sgml_files) $(sgml_static_files)
	$(rm_verbose)rm -f $@
	$(xsl_verbose)$(XSLTPROC) -o $(builddir)/$@ $(docbook_docbook_xsl) $<

$(sgml_text_target): $(generated_sgml_files) $(sgml_static_files) $(sgml_text)
	$(xsl_verbose)$(XSLTPROC) -o $(builddir)/$@.tmp.html $(docbook_docbook_xsl) $(builddir)/$@.sgml
if HAVEW3M
	$(w3m_verbose)$(W3M) -S -dump $(builddir)/$@.tmp.html > $(top_srcdir)/$@
else
	$(lynx_verbose)$(LYNX) -force_html -nolist -dump $(builddir)/$@.tmp.html > $(top_srcdir)/$@
endif
	$(rm_verbose)rm $(builddir)/$@.tmp.html
	$(touch_verbose)touch $(builddir)/$@

.sgml.1: $(generated_sgml_files) $(sgml_static_files)
	$(rm_verbose)rm -f $(srcdir)/$@
	$(xsl_verbose)$(XSLTPROC) -o $(srcdir)/$@ $(docbook_man_xsl) $(builddir)/$<

clean-local:
	rm -f $(sgml_text_target)
	rm -f $(htmldoc_DATA)
	rm -f $(generated_sgml_files)
	rm -f Doxyfile
	rm -f *.tmp.html

endif # HAVEXSLTPROC

endif # BUILDDOC

dist-hook: $(sgml_text_target) $(htmldoc_DATA) $(dist_man1_MANS)

if HAVEDOXYGEN
Doxyfile: Doxyfile.in
	$(editfile) $(srcdir)/$@.in > $(builddir)/$@

source-doc: Doxyfile
	$(DOXYGEN)

endif # HAVEDOXYGEN

if !DISABLEINSTALLDOC
if BUILDDOC

if BUILDSOURCEDOC
all-local: $(htmldoc_DATA) $(sgml_text_target) source-doc $(dist_man1_MANS)
else
all-local: $(htmldoc_DATA) $(sgml_text_target) $(dist_man1_MANS)
endif # BUILDSOURCEDOC

SUFFIXES = .sgml .sgml.in .html .1

else # BUILDDOC

if BUILDSOURCEDOC
all-local: source-doc
endif #BUILDSOURCEDOC

endif # BUILDDOC
else # !DISABLEINSTALLDOC
# For fixing bug #22
all-local: $(dist_man1_MANS)
endif # !DISABLEINSTALLDOC

maintainer-clean-local:
	rm -rf source-doc
	rm -f $(dist_man1_MANS)

