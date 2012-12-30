#!/bin/sh

had_error() {
	retval=$1
	msg=$2

	if [ $retval -ne 0 ]
	then
	    echo "$msg"
	    exit $retval
	fi
}

host_dash() {
    for c in clang++ g++-4.3 g++-4.4
    do
	make distclean
	./configure CXX=$c
	had_error $? "Error in $arch"
	    
	make
	had_error $? "Error in $c"
	
	make check
	had_error $? "Error in $c"
    done
}

AURORA_CXXFLAGS_34='-O3 -pedantic -Wall -Werror'
AURORA_CFLAGS_34="$AURORA_CXXFLAGS_34"
AURORA_CC_34=gcc34
AURORA_CXX_34=g++34
AURORA_LDFLAGS_34="-L/usr/local/lib/gcc34 -R/usr/local/lib/gcc34"

AURORA_CXXFLAGS_42='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
AURORA_CFLAGS_42="$AURORA_CXXFLAGS_42"
AURORA_CC_42=gcc42
AURORA_CXX_42=g++42
AURORA_LDFLAGS_42="-L/usr/local/lib/gcc42 -R/usr/local/lib/gcc42"

AURORA_CXXFLAGS_44="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_44="$AURORA_CXXFLAGS_42"
AURORA_CC_44=gcc44
AURORA_CXX_44=g++44
AURORA_LDFLAGS_44="-L/usr/local/lib/gcc44 -R/usr/local/lib/gcc44"

AURORA_CXXFLAGS_46="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_46="$AURORA_CXXFLAGS_42"
AURORA_CC_46=gcc46
AURORA_CXX_46=g++46
AURORA_LDFLAGS_46="-L/usr/local/lib/gcc46 -R/usr/local/lib/gcc46"

AURORA_CXXFLAGS_47="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_47="$AURORA_CXXFLAGS_42"
AURORA_CC_47=gcc47
AURORA_CXX_47=g++47
AURORA_LDFLAGS_47="-L/usr/local/lib/gcc47 -R/usr/local/lib/gcc47"

AURORA_CXXFLAGS_48="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_48="$AURORA_CXXFLAGS_42"
AURORA_CC_48=gcc48
AURORA_CXX_48=g++48
AURORA_LDFLAGS_48="-L/usr/local/lib/gcc48 -R/usr/local/lib/gcc48"

AURORA_CXXFLAGS_default="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_default="$AURORA_CXXFLAGS_42"
AURORA_CC_default=gcc
AURORA_CXX_default=g++
AURORA_LDFLAGS_default="-L/usr/lib"

AURORA_CXXFLAGS_clang="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_clang="$AURORA_CXXFLAGS_42"
AURORA_CC_clang=clang
AURORA_CXX_clang=clang++
AURORA_LDFLAGS_clang="-L/usr/lib"

host_aurora() {
    for c in default clang 34 42 44 46 47 48
    do
	gmake distclean
	./configure CXX="`eval echo \\$AURORA_CXX_$c`" \
	    CC="`eval echo \\$AURORA_CC_$c`" \
	    CFLAGS="`eval echo \\$AURORA_CFLAGS_$c`" \
	    CXXFLAGS="`eval echo \\$AURORA_CXXFLAGS_$c`" \
	    LDFLAGS="`eval echo \\$AURORA_LDFLAGS_$c`"

	had_error $? "Error in CXX=`eval echo \\$AURORA_CXX_$c` CC=`eval echo \\$AURORA_CC_$c` CFLAGS=`eval echo \\$AURORA_CFLAGS_$c` CXXFLAGS=`eval echo \\$AURORA_CXXFLAGS_$c` LDFLAGS=`eval echo \\$AURORA_LDFLAGS_$c`"
	    
	gmake -j
	had_error $? "Error in CXX=`eval echo \\$AURORA_CXX_$c` CC=`eval echo \\$AURORA_CC_$c` CFLAGS=`eval echo \\$AURORA_CFLAGS_$c` CXXFLAGS=`eval echo \\$AURORA_CXXFLAGS_$c` LDFLAGS=`eval echo \\$AURORA_LDFLAGS_$c`"
	    
	gmake check
	had_error $? "Error in CXX=`eval echo \\$AURORA_CXX_$c` CC=`eval echo \\$AURORA_CC_$c` CFLAGS=`eval echo \\$AURORA_CFLAGS_$c` CXXFLAGS=`eval echo \\$AURORA_CXXFLAGS_$c` LDFLAGS=`eval echo \\$AURORA_LDFLAGS_$c`"
    done
}

host_fish() {
    gmake distclean
    ./configure CXXFLAGS=${arch} CFLAGS=${arch}
    had_error $? "Error in $arch"
	    
    gmake
    had_error $? "Error in $arch"
	    
    gmake check
    had_error $? "Error in $arch"
}

host_odin() {
    for c in /opt/solarisstudio12.3/bin /opt/solstudio12.2/bin
    do
	for arch in -m64 -m32
	do
	    for flags in xpg4 solaris ncurses
	    do
		unset LDFLAGS CPPFLAGS
		case $flags in
		    xpg4)
			LDFLAGS="-L/usr/xpg4/lib/${arch#-m} -R/usr/xpg4/lib/${arch#-m}"
			CPPFLAGS="-I/usr/xpg4/include"
			;;
		    solaris)
			:
			;;
		    ncurses)
			CPPFLAGS="-I/usr/include/ncurses"
			;;
		esac

		export LDFLAGS
		export CPPFLAGS

		gmake distclean
		./configure CXX=${c}/CC CC=${c}/cc CFLAGS=$arch CXXFLAGS=$arch 
		had_error $? "Error in ${c}:$arch:$flags"

		gmake
		had_error $? "Error in ${c}:$arch:$flags"

		gmake check
		had_error $? "Error in ${c}:$arch:$flags"
	    done
	done
    done
}

HOST="`uname -n | sed 's/\..*//'`"
cd /home/rafisol/yapet/trunk/mockup
host_$HOST
