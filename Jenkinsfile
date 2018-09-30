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
                            steps {
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
                                    sh '$MAKE distcheck'
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
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin NEWS'
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
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin'
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

                stage("OpenBSD i386") {
					agent {
						label "openbsd&&i386"
					}
					stages {
						stage("(OB32) Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "touch README NEWS"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(OB32) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug CC=cc CXX=c++"
                                }
                            }
                        }

                        stage("(OB32) Stub Docs") {
                            steps {
                                dir("doc") {
                                    sh 'touch csv2yapet.1 yapet.1 yapet2csv.1 yapet_colors.5 yapet_config.5 csv2yapet.html DESIGN.html INSTALL.html README.Cygwin.html README.html yapet2csv.html yapet_colors.html yapet_config.html yapet.html'
                                }
                                sh 'touch DESIGN README.Cygwin'
                            }
                        }

						stage("(OB32) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }
                        
                        stage("(OB32) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("OpenBSD i386")

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
}
