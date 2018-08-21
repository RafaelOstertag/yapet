pipeline {
    agent {
        label any
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
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "touch README"
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("Build Docs") {
                            steps {
                                dir("obj/doc") {
                                    sh '$MAKE -f Makefile.doc'
                                }
                            }
                        }

                        stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                            }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
                                }
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
								sh "git log --stat > ChangeLog"
								dir("libyacurs") {
									sh "git log --stat > ChangeLog"
								}
								sh "touch README"
								sh "autoreconf -I m4 -i"
							}
						}

						stage("Configure") {
							steps {
								dir("obj") {
									sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
								}
							}
						}

						stage("Build Docs") {
							steps {
								dir("obj/doc") {
									sh '$MAKE -f Makefile.doc'
								}
							}
						}

						stage("Build") {
							steps {
								dir("obj") {
									sh '$MAKE all'
								}
							}
						}

						stage("Test") {
							steps {
								dir("obj") {
									sh '$MAKE check'
								}
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
