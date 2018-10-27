pipeline {
    agent any

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
        timeout(time: 1, unit: 'HOURS')
        timestamps()
    }

    environment {
        PEDANTIC_FLAGS = "-Wall -pedantic -Werror -O3 -Wno-unknown-pragmas -fstack-protector"
    }

    triggers {
        pollSCM '@hourly'
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
                                PATH = "$PATH:$HOME/.gem/ruby/2.5/bin"
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
                                    sshagent(['0b266ecf-fa80-4fe8-bce8-4c723f5ba47a']) {
                                        // Eventhorizon only allows sftp
                                        sh """sftp yapet-deploy@eventhorizon.dmz.kruemel.home:/var/www/jails/yapet/usr/local/www/apache24/data/downloads/ <<EOF
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

                stage("FreeBSD i386") {
                    agent {
                        label "freebsd&&i386"
                    }
                    stages {
                        stage("(FB32) Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(FB32) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug"
                                }
                            }
                        }

                        stage("(FB32) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html INSTALL.html README.html NEWS.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch NEWS'
                            }
                        }

						stage("(FB32) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }

                        stage("(FB32) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
                    }
                } // stage("FreeBSD i386")

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
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                         }

                        stage("(LX) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
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
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }
                        
                        stage("(OB64) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("OpenBSD amd64")

				stage("NetBSD") {
					agent {
						label "netbsd"
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
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                         }

                        stage("(NB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("NetBSD")
    		} // parallel
        } // stage("OS Build")
    } // stages
}
