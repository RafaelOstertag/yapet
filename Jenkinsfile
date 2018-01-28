node("freebsd") {
    stage("checkout") {
	checkout scm
    }

    // The yapet repository contains a submodule, however, multibranch
    // pipeline does not initialize the submodules. We use the
    // workaround as mentioned in
    //  https://issues.jenkins-ci.org/browse/JENKINS-32658
    stage("submodules") {
	sh "git submodule update --init"
    }
	

    stage("autoconf") {
	sh "touch README ChangeLog libyacurs/ChangeLog"
	sh "autoreconf -I m4 -i"
    }
}
