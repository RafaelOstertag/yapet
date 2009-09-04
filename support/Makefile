# $Id$
#
# Uses sed to replace stuff in the html files...

BUILDDIR=..

VPATH=.:$(BUILDDIR)/doc
FILES=csv2yapet.shtml DESIGN.shtml INSTALL.shtml README.shtml yapet.shtml README.Cygwin.shtml

SED = /opt/guengel/bin/sed -r -e 's/<ul[ ]+type=\"[a-z]+\">/<ul>/g' \
	-e 's|[ ]+lang=\"en\"||g' \
	-e 's%>(INSTALL|DESIGN|README)<%><a href="\L\1.shtml" title="Read the \U\1\E File Online">\U\1\E</a><%g' \
	-e 's%>(README\.Cygwin)<%><a href="cygwin.shtml" title="Read the \1 File Online">\1</a><%g' \
	-e 's%(yapet|csv2yapet)\.html%<a href="\1.shtml" title="Read the \1 Man Page Online">\1.html</a>%g' \
	-e 's%(README|DESIGN)\.html%<a href="\L\1\E.shtml" title="Read the \1 Files Online">\1.html</a>%g' \
	-e 's|(yapet-[0-9]\.[0-9]\.tar\.gz)|<a href="downloads/\1" title="Download YAPET">\1</a>|g' \
	-e 's%>(yapet|csv2yapet)</span>\(1\)</span>%><a href="\1.shtml" title="Read the \1 Man Page Online">\1</a></span>(1)</span>%g' \
	-e 's|\$$Id|\&\#36;Id|g'
SED_LANG = /opt/guengel/bin/sed -r -e 's|[ ]+lang=\"en\"||g'


all: $(FILES)

.sgml.html:
	cd $(BUILDDIR)/doc ; \
	$(MAKE) $@

.html.shtml:
	$(SED) $< > $@
#	-tidy -utf8 -i -q -o $@.tmp -w $@
# Tidy will put back lang="en"
#	$(SED_LANG) $@.tmp > $@
	rm -f $@.tmp

clean:
	rm -f *.shtml
	rm -f *~

.SUFFIXES: .shtml .html .sgml

