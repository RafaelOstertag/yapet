#!/bin/sh

for h in fish dash debian32 freebsd32 openbsd32 starchild odin
do
    ssh -t $h "cd $HOME/yapet/trunk/mockup ; if [ -x /usr/xpg4/bin/sh ] ; then /usr/xpg4/bin/sh checkall.sh ; else /bin/sh checkall.sh ; fi"
    if [ $? -ne 0 ]
    then
	echo "ERROR on $h"
	exit 1
    fi
done

exit 0
