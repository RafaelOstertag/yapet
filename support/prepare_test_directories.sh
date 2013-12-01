#!/bin/sh

set -e

BASEDIRECTORY=$HOME/yapet-test

HOSTS="aurora freebsd32 fish openbsd32 dash hyperion debian32 wheezy abraxas netbsd32"

for h in $HOSTS
do
    [ ! -d "$BASEDIRECTORY" ] && mkdir "$BASEDIRECTORY"
    [ ! -d "$BASEDIRECTORY/$h" ] && svn co https://gizmo.kruemel.home/svn/swprojects/yapet/trunk "$BASEDIRECTORY/$h"
    [ -d "$BASEDIRECTORY/$h" ] && { cd "$BASEDIRECTORY/$h" ; svn update ; }

    cd "$BASEDIRECTORY/$h"
    touch ChangeLog README libyacurs/ChangeLog libyacurs/README
    autoreconf -fi
done
