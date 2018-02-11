# $Id$


EXTRA_DIST = README.html DESIGN.html yapet.html Doxyfile	\
csv2yapet.html yapet2csv.html yapet_colors.html yapet_config.html Makefile.doc.in Makefile.doc.am

# For fixing bug #22
dist_man1_MANS = yapet.1
dist_man5_MANS = yapet_colors.5 yapet_config.5
if BUILDCONVERTS
dist_man1_MANS += csv2yapet.1 yapet2csv.1
endif # BUILDCONVERTS

if !DISABLEINSTALLDOC
htmldoc_DATA = README.html DESIGN.html yapet.html yapet_colors.html yapet_config.html

if BUILDCONVERTS
htmldoc_DATA += csv2yapet.html yapet2csv.html
endif # BUILDCONVERTS

htmldocdir = $(docdir)/html

endif # !DISABLEINSTALLDOC

distclean-local:
	rm -f Makefile.doc
