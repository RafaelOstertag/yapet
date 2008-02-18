# $Id$

EXTRA_DIST = Doxyfile.in README.sgml.in INSTALL.sgml.in preamble.sgml html-doc

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

if HAVEDOCBOOK2TXT
$(top_srcdir)/README: README.sgml
	$(DOCBOOK2TXT) $(srcdir)/README.sgml
	mv $(srcdir)/README.txt $(top_srcdir)/README

$(top_srcdir)/INSTALL: INSTALL.sgml
	$(DOCBOOK2TXT) $(srcdir)/INSTALL.sgml
	mv $(srcdir)/INSTALL.txt $(top_srcdir)/INSTALL
else
if HAVEXSLTPROC
$(top_srcdir)/README: README.sgml
	$(XSLTPROC) -o $(srcdir)/README.html $(XSLFILE) $(srcdir)/README.sgml
	$(LYNX) -nolist -dump $(srcdir)/README.html > $@
	rm -f $(srcdir)/README.html

$(top_srcdir)/INSTALL: INSTALL.sgml
	$(XSLTPROC) -o $(srcdir)/INSTALL.html $(XSLFILE) $(srcdir)/INSTALL.sgml
	$(LYNX) -nolist -dump $(srcdir)/INSTALL.html > $@
	rm -f $(srcdir)/INSTALL.html
else
$(top_srcdir)/README: README.sgml

$(top_srcdir)/INSTALL: INSTALL.sgml

endif # if HAVEXSLTPROC
endif # if HAVEDOCBOOK2TXT

if HAVEDOXYGEN
html-doc: Doxyfile
	$(DOXYGEN)
else
html-doc: Doxyfile

endif

dist-hook: html-doc $(top_srcdir)/README $(top_srcdir)/INSTALL
all-local: $(top_srcdir)/README $(top_srcdir)/INSTALL

maintainer-clean-local:
	rm -rf html-doc
	rm -f README.sgml
	rm -f INSTALL.sgml
