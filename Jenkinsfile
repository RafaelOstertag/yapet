@Library('yapetJenkins') _

properties([buildDiscarder(logRotator(artifactDaysToKeepStr: '',
				      artifactNumToKeepStr: '',
				      daysToKeepStr: '', numToKeepStr:
				      '10'))])
node("freebsd") {
    yapet.checkout()
    yapet.autoconf()
    yapet.build "FreeBSD"
}

node("openbsd") {
    yapet.checkout()
    yapet.autoconf()
    yapet.build "OpenBSD"
}

node("netbsd") {
    yapet.checkout()
    yapet.autoconf()
    yapet.build "NetBSD"
}

node("linux") {
    yapet.checkout()
    yapet.autoconf()
    yapet.build "Linux"
}
