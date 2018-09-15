yapet_builddir= $(top_builddir)/src

SUBDIRS = csv import
EXTRA_DIST = testimport.csv tests.h

check-local:
	rm -f /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	$(yapet_builddir)/converters/csv2yapet/csv2yapet -p secret -s ',' $(srcdir)/testimport.csv /tmp/yapettestimport.pet
	$(yapet_builddir)/converters/yapet2csv/yapet2csv -p secret -s ',' /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	diff $(srcdir)/testimport.csv /tmp/yapettestexport.csv
