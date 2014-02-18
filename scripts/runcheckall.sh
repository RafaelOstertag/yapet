#!/bin/sh

TESTDIR="${HOME}/yapet-test"
TESTDIR_SRC="${TESTDIR}/yapet-src"
if [ ! -d "${TESTDIR_SRC}" ]
then
    svn co https://gizmo.kruemel.home/svn/swprojects/yapet/trunk "${TESTDIR_SRC}"
else
    cd "${TESTDIR_SRC}"
    svn update || exit 1
fi

cd "${TESTDIR_SRC}"
touch ChangeLog libyacurs/ChangeLog
touch README libyacurs/README
autoreconf -fi || exit 1

for h in fish dash debian32 freebsd32 openbsd32 netbsd32 abraxas aurora starchild odin wheezy puffy hyperion merlin ion grimreaper
do
    TESTDIRHOST="${TESTDIR}/${h}"

    if [ ! -d "${TESTDIR}" ]
    then
	mkdir "${TESTDIR}"
    fi

    if [ -d "${TESTDIRHOST}" ]
    then
	rm -rf "${TESTDIRHOST}"/*
    else
	mkdir "${TESTDIRHOST}"
    fi
    
    cd "${TESTDIRHOST}" || exit 1


    xterm -T "${h}" -n "${h}" -e "ssh -t $h 'cd ${TESTDIRHOST} ; if [ -x /usr/xpg4/bin/sh ] ; then /usr/xpg4/bin/sh ${TESTDIR_SRC}/scripts/checkall.sh ; else /bin/sh ${TESTDIR_SRC}/scripts/checkall.sh ; fi ; echo '============= DONE =============' ; read BLA'" &
done

exit 0
