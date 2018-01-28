buildProfiles = [
    "default" : [
	"env": [
	    "CC=cc",
	    "CXX=c++",
	    "CPP=cpp",
	    "LDFLAGS=",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-clang50-ports" : [
	"env": [
	    "CC=clang50",
	    "CXX=clang++50",
	    "CPP=clang-cpp50",
	    "LDFLAGS=-L/usr/local/llvm50/lib -Wl,-rpath -Wl,/usr/local/llvm50/lib",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-clang34-ports" : [
	"env": [
	    "CC=clang34",
	    "CXX=clang++34",
	    "CPP=clang-cpp34",
	    "LDFLAGS=-L/usr/local/llvm34/lib -Wl,-rpath -Wl,/usr/local/llvm34/lib",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-clang35-ports" : [
	"env": [
	    "CC=clang35",
	    "CXX=clang++35",
	    "CPP=clang-cpp35",
	    "LDFLAGS=-L/usr/local/llvm35/lib -Wl,-rpath -Wl,/usr/local/llvm35/lib",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-clang38-ports" : [
	"env": [
	    "CC=clang38",
	    "CXX=clang++38",
	    "CPP=clang-cpp38",
	    "LDFLAGS=-L/usr/local/llvm38/lib -Wl,-rpath -Wl,/usr/local/llvm38/lib",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc47-ports" : [
	"env": [
	    "CC=gcc47",
	    "CXX=g++47",
	    "CPP=cpp47",
	    "LDFLAGS=-L/usr/local/lib/gcc47 -Wl,-rpath -Wl,/usr/local/lib/gcc47",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc48-ports" : [
	"env": [
	    "CC=gcc48",
	    "CXX=g++48",
	    "CPP=cpp48",
	    "LDFLAGS=-L/usr/local/lib/gcc48 -Wl,-rpath -Wl,/usr/local/lib/gcc48",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc49-ports" : [
	"env": [
	    "CC=gcc49",
	    "CXX=g++49",
	    "CPP=cpp49",
	    "LDFLAGS=-L/usr/local/lib/gcc49 -Wl,-rpath -Wl,/usr/local/lib/gcc49",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc5-ports" : [
	"env": [
	    "CC=gcc5",
	    "CXX=g++5",
	    "CPP=cpp5",
	    "LDFLAGS=-L/usr/local/lib/gcc5 -Wl,-rpath -Wl,/usr/local/lib/gcc5",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc6-ports" : [
	"env": [
	    "CC=gcc6",
	    "CXX=g++6",
	    "CPP=cpp6",
	    "LDFLAGS=-L/usr/local/lib/gcc6 -Wl,-rpath -Wl,/usr/local/lib/gcc6",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ],
    "fb-gcc7-ports" : [
	"env": [
	    "CC=gcc7",
	    "CXX=g++7",
	    "CPP=cpp7",
	    "LDFLAGS=-L/usr/local/lib/gcc7 -Wl,-rpath -Wl,/usr/local/lib/gcc7",
	    "CFLAGS=",
	    "CXXFLAGS="
	],
	"flags": [
	    "--disable-silent-rules",
	    "--enable-debug"
	]
    ]
]

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

void build(profileName) {
    environmentVariables = buildProfiles[profileName].env
    stage(makeStageName("configure " + profileName)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh "../configure " + buildProfiles[profileName].flags.join(" ")
	    }
	}
    }
    stage(makeStageName("docs " + profileName)) {
	dir (objectDirectoryName + '/doc') {
	    withEnv(environmentVariables) {
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; if [ -f /usr/local/share/xml/catalog ] ; then  XML_CATALOG_FILES=/usr/local/share/xml/catalog ; fi ; $MAKE -f Makefile.doc'
	    }
	}
    }
    
    stage(makeStageName("build " + profileName)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE all'
	    }
	}
    }

    stage(makeStageName("check " + profileName)) {
	dir (objectDirectoryName) {
	    withEnv(environmentVariables) {
		sh 'if which gmake ; then MAKE=gmake ; else MAKE=make; fi ; $MAKE check'
	    }
	}
    }
}

node("freebsd") {
    checkout()
    autoconf()
    build "default"
}

node("openbsd") {
    checkout()
    autoconf()
    build "default"
}

node("netbsd") {
    checkout()
    autoconf()
    build "default"
}

node("linux") {
    checkout()
    autoconf()
    build "default"
}
