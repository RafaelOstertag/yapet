# -*- mode: makefile -*-
#
# $Id$

configure: configure.ac config.h.in aclocal.m4 ltmain.sh Makefile.in missing ar-lib depcomp
	autoconf

aclocal.m4: configure.ac m4/libtool.m4 m4/ltoptions.m4 m4/ltversion.m4 m4/ltsugar.m4 
	aclocal -I m4

config.h.in: configure.ac aclocal.m4
	autoheader

ltmain.sh: m4/libtool.m4 m4/ltoptions.m4 m4/ltversion.m4 m4/ltsugar.m4
	libtoolize -c -i

m4/libtool.m4:
	libtoolize -c -i

m4/ltoptions.m4:
	libtoolize -c -i

m4/ltversion.m4:
	libtoolize -c -i

m4/ltsugar.m4:
	libtoolize -c -i

Makefile.in missing ar-lib depcomp: configure.ac Makefile.am NEWS README AUTHORS ChangeLog src/Makefile.am tests/Makefile.am
	automake -a -c -W all --gnu

NEWS:
	touch $@

README:
	touch $@

AUTHORS:
	touch $@

ChangeLog:
	touch $@

clean:
	rm -f aclocal.m4 config.guess config.sub configure install-sh depcomp config.h.in missing ltmain.sh ar-lib config.status stamp-h1 m4/* Makefile.in src/Makefile.in tests/Makefile.in
	rm -rf autom4te.cache
