pipeline {
    agent any

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
        timeout(time: 1, unit: 'HOURS')
        timestamps()
    }

    environment {
        PEDANTIC_FLAGS = "-Wall -pedantic -Werror -O3 -Wno-unknown-pragmas"
    }

    triggers {
        pollSCM '@hourly'
    }

    stages {
        stage("OS Build") {
            parallel {
                stage("FreeBSD") {
                    agent {
                        label "freebsd"
                    }
                    stages {
                        stage("(FB) Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(FB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug"
                                }
                            }
                        }

                        stage("(FB) Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }
						 stage("(FB) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                         }

                        stage("(FB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
                    }
                } // stage("FreeBSD")

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
                                sh "touch README"
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
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin'
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

				stage("OpenBSD") {
					agent {
						label "openbsd"
					}
					stages {
						stage("(OB) Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(OB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug CC=cc CXX=c++"
                                }
                            }
                        }

                        stage("(OB) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin'
                            }
                        }
						 stage("(OB) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                         }

                        stage("(OB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("OpenBSD")

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
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(NB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug LDFLAGS='-L/usr/pkg/lib -R/usr/pkg/lib'"
                                }
                            }
                        }

                        stage("(NB) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin'
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
