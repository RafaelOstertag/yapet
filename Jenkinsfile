pipeline {
    agent {
        any
    }

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
    }

    stages {
        stage("OS Build") {
            parallel {
                stage("FreeBSD") {
                    agent {
                        label "freebsd"
                    }
                    stages {
                        stage("Bootstrap Build") {
                            steps {
				echo "one"
                            }
                        }

                    }
                }

		stage("Linux") {
		    agent {
			label "linux"
		    }
		    stages {
			stage("Bootstrap Build") {
			    steps {
				echo "two"
			    }
			}
		    }
		}
            }
        }
    }

    post {
        always {
            // If distcheck fails, it leaves certain directories with read-only permissions.
            // We unconditionally set write mode
            dir("obj") {
                sh "chmod -R u+w ."
            }

            cleanWs notFailBuild: true
        }
    }

}
