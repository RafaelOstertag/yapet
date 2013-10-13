# -*- makefile -*-
#
# $Id$

VPATH=.:m4

all: configure libyacurs/configure

libyacurs/configure:
	cd libyacurs && make -f Makefile.boot

configure: configure.ac config.h.in aclocal.m4 ltmain.sh Makefile.in missing ar-lib depcomp nls.m4 po.m4
	autoconf -f

aclocal.m4: configure.ac libtool.m4 ltoptions.m4 ltversion.m4 ltsugar.m4 
	aclocal -I m4

config.h.in: configure.ac aclocal.m4
	autoheader

ltmain.sh libtool.m4 ltoptions.m4 ltversion.m4 ltsugar.m4: configure.ac
	libtoolize -c -i --force

config.rpath:
	touch $@

nls.m4 po.m4:
	gettextize -f

Makefile.in missing ar-lib depcomp: configure.ac Makefile.am NEWS README AUTHORS ./ChangeLog config.rpath crypt/Makefile.am tests/Makefile.am doc/Makefile.am csv2yapet/Makefile.am yapet/pwgen/Makefile.am yapet/Makefile.am yapet/glue/Makefile.am yapet/ui/Makefile.am
	automake -a -c -W all --gnu -f

NEWS:
	touch $@

README:
	touch $@

AUTHORS:
	touch $@

./ChangeLog:
	touch $@

clean:
	rm -f aclocal.m4 config.guess config.sub configure install-sh depcomp config.h.in missing ltmain.sh ar-lib config.status stamp-h1 Makefile.in yapet/Makefile.in tests/Makefile.in config.rpath yapet/glue/Makefile.in yapet/pwgen/Makefile.in
	rm -rf autom4te.cache
