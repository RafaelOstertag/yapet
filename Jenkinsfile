properties([buildDiscarder(logRotator(artifactDaysToKeepStr: '',
				      artifactNumToKeepStr: '',
				      daysToKeepStr: '', numToKeepStr:
				      '10'))])

void checkout() {
    stage("checkout") {
	checkout scm
    }
}

void autoconf() {
    stage("autoconf") {
	touch "README"
	touch "ChangeLog"
	touch "libyacurs/ChangeLog"
	// AUTOCONF_VERSION is used on OpenBSD.
	withEnv(["AUTOCONF_VERSION=2.69"]) {
	    sh "autoreconf -I m4 -i"
	}
    }
}

node("openbsd") {
    checkout()
    autoconf()
}

node("netbsd") {
    checkout()
    autoconf()
}

node("linux") {
    checkout()
    autoconf()
}

node("freebsd") {
    checkout()
    autoconf()

    stage("prepare object directory") {
	dir ('obj-dir') {
            touch "dummy"
	}
    }

    stage("configure") {
	dir ('obj-dir') {
	    sh "../configure"
	}
    }

    stage("docs") {
	dir ('obj-dir/doc') {
	    withEnv(['XML_CATALOG_FILES=/usr/local/share/xml/catalog']) {
		sh "gmake -f Makefile.doc"
	    }
	}
    }

    stage("build") {
	dir ('obj-dir') {
	    sh "gmake all"
	}
    }

    stage("check") {
	dir ('obj-dir') {
	    sh "gmake check"
	}
    }
}
