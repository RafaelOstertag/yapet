properties([buildDiscarder(logRotator(artifactDaysToKeepStr: '',
				      artifactNumToKeepStr: '',
				      daysToKeepStr: '', numToKeepStr:
				      '10'))])
void checkout() {
    stage("checkout " + NODE_NAME) {
	checkout scm
    }
}

void autoconf() {
    stage("autoconf " + NODE_NAME) {
	touch "README"
	touch "ChangeLog"
	touch "libyacurs/ChangeLog"
	// AUTOCONF_VERSION and AUTOMAKE_VERSION is used on OpenBSD.
	withEnv(["AUTOCONF_VERSION=2.69", "AUTOMAKE_VERSION=1.15"]) {
	    sh "autoreconf -I m4 -i"
	}
    }
}

stage("distribute") {
    parallel (
	"openbsd": {
	    node("openbsd") {
		checkout()
		autoconf()
	    }
	},
	"netbsd": {
	    node("netbsd") {
		checkout()
		autoconf()
	    }
	},
	"linux": {
	    node("linux") {
		checkout()
		autoconf()
	    }
	},
	"freebsd": {
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
	}
    )
}
