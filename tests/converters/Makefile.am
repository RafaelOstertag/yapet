yapet_srcdir = $(top_srcdir)/srcdir
yapet_libs_srcdir = $(yapet_srcdir)/libs
yapet_builddir= $(top_builddir)/src

EXTRA_DIST = testimport.csv

check-local:
	rm -f /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	$(yapet_builddir)/converters/csv2yapet/csv2yapet -p secret -s ',' $(srcdir)/testimport.csv /tmp/yapettestimport.pet
	$(yapet_builddir)/converters/yapet2csv/yapet2csv -p secret -s ',' /tmp/yapettestimport.pet /tmp/yapettestexport.csv
	diff $(srcdir)/testimport.csv /tmp/yapettestexport.csv
