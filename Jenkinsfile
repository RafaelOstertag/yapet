pipeline {
    agent none

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
        timeout(time: 1, unit: 'HOURS')
        timestamps()
        disableConcurrentBuilds()
    }

    environment {
        PEDANTIC_FLAGS = "-Wall -pedantic -Werror -O3 -Wno-unknown-pragmas"
        CODE_INSTRUMENTATION_FLAGS = "-fstack-protector-strong -fsanitize=address"
    }

    triggers {
        pollSCM '@hourly'
        cron '@daily'
    }

    stages {
        stage("OS Build") {
            parallel {
                stage("FreeBSD amd64") {
                    agent {
                        label "freebsd&&amd64"
                    }
                    stages {
                        stage("(FB64) Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(FB64) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug"
                                }
                            }
                        }

                        stage("(FB64) Build Docs") {
                            environment {
                                PATH = "$PATH:$HOME/.gem/ruby/2.6/bin"
                            }
                            steps {
                                sh 'gem install --user-install asciidoctor'
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }

						stage("(FB64) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }

                        stage("(FB64) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }

                        stage("Build distribution") {
                            when { 
                                branch 'release/*'
                            }

                            steps {
                                dir("obj") {
                                    sh '$MAKE distcheck DISTCHECK_CONFIGURE_FLAGS="--enable-nls  --with-libiconv-prefix=/usr/local --with-libintl-prefix=/usr/local"'
                                    sshagent(['897482ed-9233-4d56-88c3-254b909b6316']) {
                                        sh """sftp ec2-deploy@ec2-52-29-59-221.eu-central-1.compute.amazonaws.com:/data/www/yapet.guengel.ch/downloads <<EOF
put yapet-*.tar.*
EOF
"""
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
                    }
                } // stage("FreeBSD amd64")

				stage("Linux") {
					agent {
						label "linux"
					}
					stages {
						stage("(LX) Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(LX) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug"
                                }
                            }
                        }

                        stage("(LX) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html INSTALL.html README.html NEWS.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch NEWS'
                            }
                        }
						 stage("(LX) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS} ${CODE_INSTRUMENTATION_FLAGS}"'
                                }
                             }
                         }

                        stage("(LX) Test") {
                            environment {
                                EXTRA_LD_PRELOAD = "/usr/lib/gcc/x86_64-linux-gnu/6/libasan.so:"
                            }
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS} ${CODE_INSTRUMENTATION_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("Linux")

				stage("OpenBSD amd64") {
					agent {
						label "openbsd&&amd64"
					}
					stages {
						stage("(OB64) Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(OB64) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug CC=cc CXX=c++"
                                }
                            }
                        }

                        stage("(OB64) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html INSTALL.html README.html NEWS.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                            }
                        }

						stage("(OB64) Build") {
                            steps {
                                dir("obj") {
                                    // OpenBSD 6.4 does not support -fsanitize=address, so no code instrumentation
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }
                        
                        stage("(OB64) Test") {
                            steps {
                                dir("obj") {
                                    // OpenBSD 6.4 does not support -fsanitize=address, so no code instrumentation
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("OpenBSD amd64")

			    stage("NetBSD") {
                    agent {
						label "netbsd&&amd64"
					}
    			    stages {
						stage("(NB) Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(NB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug LDFLAGS='-L/usr/pkg/lib -R/usr/pkg/lib' ARGON2_CFLAGS='-I/usr/pkg/include' ARGON2_LIBS='-L/usr/pkg/lib -largon2'"
                                }
                            }
                        }

                        stage("(NB) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html INSTALL.html README.html NEWS.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                            }
                        }
						 stage("(NB) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS} ${CODE_INSTRUMENTATION_FLAGS}"'
                                }
                             }
                         }

                        stage("(NB) Test") {
                            environment {
                                EXTRA_LD_PRELOAD = "/usr/lib/libasan.so:"
                            }
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS} ${CODE_INSTRUMENTATION_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("NetBSD")
    		} // parallel
        } // stage("OS Build")
    } // stages

    post {
        always {
            mail to: "rafi@guengel.ch",
                    subject: "${JOB_NAME} (${BRANCH_NAME};${env.BUILD_DISPLAY_NAME}) -- ${currentBuild.currentResult}",
                    body: "Refer to ${currentBuild.absoluteUrl}"
        }
    }
}
