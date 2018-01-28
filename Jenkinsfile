node("freebsd") {
    stage("checkout") {
	checkout scm
    }

    stage("autoconf") {
	sh "touch README ChangeLog libyacurs/ChangeLog"
	sh "autoreconf -I m4 -i"
    }
}
