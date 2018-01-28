properties([buildDiscarder(logRotator(artifactDaysToKeepStr: '',
				      artifactNumToKeepStr: '',
				      daysToKeepStr: '', numToKeepStr:
				      '10'))])

void makeStageName(name) {
    return name + " " + NODE_NAME
}

void checkout() {
    stage(makeStageName("checkout")) {
	checkout scm
    }
}

void autoconf() {
    stage(makeStageName("autoconf")) {
	touch "README"
	touch "ChangeLog"
	touch "libyacurs/ChangeLog"
	// AUTOCONF_VERSION and AUTOMAKE_VERSION is used on OpenBSD.
	withEnv(["AUTOCONF_VERSION=2.69", "AUTOMAKE_VERSION=1.15"]) {
	    sh "autoreconf -I m4 -i"
	}
    }
}

void buildWithSystemDefaults() {
    stage(makeStageName("configure")) {
	dir ('obj-dir-system-default') {
	    sh "../configure"
	}
    }
    stage(makeStageName("docs")) {
	dir ('obj-dir/doc') {
	    withEnv(['XML_CATALOG_FILES=/usr/local/share/xml/catalog']) {
		sh "gmake -f Makefile.doc"
	    }
	}
    }
    
    stage(makeStageName("build")) {
	dir ('obj-dir') {
	    sh "gmake all"
	}
    }

    stage(makeStageName("check")) {
	dir ('obj-dir') {
	    sh "gmake check"
	}
    }
}

stage("distribute") {
    parallel (
	"openbsd": {
	    node("openbsd") {
		checkout()
		autoconf()
		buildWithSystemDefaults()
	    }
	},
	"netbsd": {
	    node("netbsd") {
		checkout()
		autoconf()
		buildWithSystemDefaults()
	    }
	},
	"linux": {
	    node("linux") {
		checkout()
		autoconf()
		buildWithSystemDefaults()
	    }
	},
	"freebsd": {
	    node("freebsd") {
		checkout()
		autoconf()
		buildWithSystemDefaults()
	    }
	}
    )
}
