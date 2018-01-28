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

void build(objectDirectoryName, cc="cc", cxx="c++", ldflags="") {
    environmentVariables = [
	"CC="+cc,
	"CXX="+cxx,
	"LDFLAGS="+ldflags
    ]
    
    stage(makeStageName("configure " + cxx)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh "../configure"
	    }
	}
    }
    stage(makeStageName("docs " + cxx)) {
	dir (objectDirectoryName + '/doc') {
	    withEnv(environmentVariables) {
		sh "if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE -f Makefile.doc"
	    }
	}
    }
    
    stage(makeStageName("build " + cxx)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh "if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE all"
	    }
	}
    }

    stage(makeStageName("check " + cxx)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh "if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE check"
	    }
	}
    }
}

void buildWithSystemDefaults() {
    build 'obj-dir-system-default'
}

node("openbsd") {
    checkout()
    autoconf()
    buildWithSystemDefaults()
}

node("netbsd") {
    checkout()
    autoconf()
    buildWithSystemDefaults()
}

node("linux") {
    checkout()
    autoconf()
    buildWithSystemDefaults()
}

node("freebsd") {
    checkout()
    autoconf()
    buildWithSystemDefaults()
    build "clang-5", "clang50", "clang++50", "-L/usr/local/llvm50/lib -Wl,-rpath -Wl,/usr/local/llvm50/lib"
}

