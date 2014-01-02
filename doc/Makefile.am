# $Id$


EXTRA_DIST = README.html DESIGN.html yapet.html Doxyfile csv2yapet.html Makefile.doc.in Makefile.doc.am

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

distclean-local:
	rm -f Makefile.doc
