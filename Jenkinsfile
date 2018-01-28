properties([buildDiscarder(logRotator(artifactDaysToKeepStr: '',
				      artifactNumToKeepStr: '',
				      daysToKeepStr: '', numToKeepStr:
				      '10'))])
emailext recipientProviders: [[$class: 'CulpritsRecipientProvider'], [$class: 'DevelopersRecipientProvider']]

node("freebsd") {
    stage("checkout") {
	checkout scm
    }

    stage("autoconf") {
	touch "README"
	touch "ChangeLog"
	touch "libyacurs/ChangeLog"
	sh "autoreconf -I m4 -i"
    }

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
	    sh "gmake -f Makefile.doc"
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
