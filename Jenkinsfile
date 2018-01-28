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
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; if [ -f /usr/local/share/xml/catalog ] ; then  XML_CATALOG_FILES=/usr/local/share/xml/catalog ; fi ; $MAKE -f Makefile.doc'
	    }
	}
    }
    
    stage(makeStageName("build " + cxx)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE all'
	    }
	}
    }

    stage(makeStageName("check " + cxx)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE check'
	    }
	}
    }
}

void buildWithSystemDefaults() {
    build 'obj-dir-system-default'
}

node("freebsd") {
    checkout()
    autoconf()
    buildWithSystemDefaults()
    build "clang-5", "clang50", "clang++50", "-L/usr/local/llvm50/lib -Wl,-rpath -Wl,/usr/local/llvm50/lib"
    // Configure claims it does not support c++98
    //build "clang-33", "clang33", "clang++33", "-L/usr/local/llvm33/lib -Wl,-rpath -Wl,/usr/local/llvm33/lib"
    build "clang-34", "clang34", "clang++34", "-L/usr/local/llvm34/lib -Wl,-rpath -Wl,/usr/local/llvm34/lib"
    build "clang-35", "clang35", "clang++35", "-L/usr/local/llvm35/lib -Wl,-rpath -Wl,/usr/local/llvm35/lib"
    build "clang-38", "clang38", "clang++38", "-L/usr/local/llvm38/lib -Wl,-rpath -Wl,/usr/local/llvm38/lib"
    // Path to cpp is messed up.
    //build "gcc-47", "gcc47", "gcc++47", "-L/usr/local/lib/gcc47 -Wl,-rpath -Wl,/usr/local/lib/gcc47"
    build "gcc-48", "gcc48", "gcc++48", "-L/usr/local/lib/gcc48 -Wl,-rpath -Wl,/usr/local/lib/gcc48"
    build "gcc-49", "gcc49", "gcc++49", "-L/usr/local/lib/gcc49 -Wl,-rpath -Wl,/usr/local/lib/gcc49"
    build "gcc-5", "gcc5", "gcc++5", "-L/usr/local/lib/gcc5 -Wl,-rpath -Wl,/usr/local/lib/gcc5"
    build "gcc-6", "gcc6", "gcc++6", "-L/usr/local/lib/gcc6 -Wl,-rpath -Wl,/usr/local/lib/gcc6"
    build "gcc-7", "gcc7", "gcc++7", "-L/usr/local/lib/gcc7 -Wl,-rpath -Wl,/usr/local/lib/gcc7"
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
