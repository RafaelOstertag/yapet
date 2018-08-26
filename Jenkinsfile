pipeline {
    agent any

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
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
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
                                    sh '$MAKE all'
                                }
                             }
                         }

                        stage("(FB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(LX) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("(LX) Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }
						 stage("(LX) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                         }

                        stage("(LX) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(OB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CC=cc CXX=c++ CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("(OB) Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }
						 stage("(OB) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                         }

                        stage("(OB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
                                }
                            }
                        }
					}
				} // stage("OpenBSD")

				stage("Solaris") {
					agent {
						label "solaris"
					}
					stages {
						stage("(SOL) Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(SOL) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("(SOL) Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }
						 stage("(SOL) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                         }

                        stage("(SOL) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
                                }
                            }
                        }
					}
				} // stage("Solaris")

				stage("NetBSD") {
					agent {
						label "netbsd"
					}
					stages {
						stage("(NB) Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("(NB) Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("(NB) Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }
						 stage("(NB) Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                         }

                        stage("(NB) Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
