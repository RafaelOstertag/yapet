// -*- c++ -*-
//
// $Id$
//
// This file holds defines common to all tests
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "partdec.h"
#include "structs.h"
#include "key.h"
#include "crypt.h"

#ifndef ROUNDS
# define ROUNDS 5000
#endif

#define FN "encryptiontest.gps"

#ifndef NAME
# define NAME "Test name %d"
#endif

#ifndef HOST
# define HOST "Test host %d"
#endif

#ifndef UNAME
# define UNAME "Test username %d"
#endif

#ifndef PW
# define PW "Test password %d"
#endif

#ifndef COMMENT
# define COMMENT "Test comment %d"
#endif

inline
void progress() {
    static int n = 0;

    // I change the progress indicator to somewhat less verbose, since the
    // above indicator looks terrible in build logs of automated builds, e.g.:
    // https://buildd.debian.org/pkg.cgi?pkg=yapet
    if ( (n % 100) == 0 && ! ( (n % 1000) == 0) ) {
        std::cout << ".";
        std::cout.flush();
    }

    if ( (n % 1000) == 0 && n != 0) {
        std::cout << "#";
        std::cout.flush();
    }

    n++;
}

inline
void print_record (YAPET::PartDec& pd, const YAPET::Key& key) {
    YAPET::Crypt crypt (key);
    std::cout << "PartDec Name:\t" << pd.getName() << std::endl;
    const YAPET::BDBuffer& enc_rec = pd.getEncRecord();
    YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = crypt.decrypt<YAPET::PasswordRecord> (enc_rec);
    YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;
    std::cout << "\tName:\t" << ptr_pw->name << std::endl;
    std::cout << "\tHost:\t" << ptr_pw->host << std::endl;
    std::cout << "\tUname:\t" << ptr_pw->username << std::endl;
    std::cout << "\tPW:\t" << ptr_pw->password << std::endl;
    std::cout << "\tCMT:\t" << ptr_pw->comment << std::endl;
    std::cout << std::endl;
    delete ptr_dec_rec;
}

inline
void check_record (YAPET::PartDec& pd, const YAPET::Key& key, int rec_no) {
    progress();
    YAPET::Crypt crypt (key);
    char _name[YAPET::NAME_SIZE];
    char _host[YAPET::HOST_SIZE];
    char _uname[YAPET::USERNAME_SIZE];
    char _pw[YAPET::PASSWORD_SIZE];
    char _cmt[YAPET::COMMENT_SIZE];
    snprintf (_name, YAPET::NAME_SIZE, NAME, rec_no);
    snprintf (_host, YAPET::HOST_SIZE, HOST, rec_no);
    snprintf (_uname, YAPET::USERNAME_SIZE, UNAME, rec_no);
    snprintf (_pw, YAPET::PASSWORD_SIZE, PW, rec_no);
    snprintf (_cmt, YAPET::COMMENT_SIZE, COMMENT, rec_no);

    if (strncmp (_name, (char*) pd.getName(), YAPET::NAME_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("Name does not match");
    }

    const YAPET::BDBuffer& enc_rec = pd.getEncRecord();

    YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = crypt.decrypt<YAPET::PasswordRecord> (enc_rec);

    YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

    if (strncmp (_name, (char*) ptr_pw->name, YAPET::NAME_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("Name does not match");
    }

    if (strncmp (_host, (char*) ptr_pw->host, YAPET::HOST_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("Host does not match");
    }

    if (strncmp (_uname, (char*) ptr_pw->username, YAPET::USERNAME_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("username does not match");
    }

    if (strncmp (_pw, (char*) ptr_pw->password, YAPET::PASSWORD_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("password does not match");
    }

    if (strncmp (_cmt, (char*) ptr_pw->comment, YAPET::COMMENT_SIZE) != 0) {
        print_record (pd, key);
        throw std::logic_error ("comment does not match");
    }

    delete ptr_dec_rec;
    /*    std::cout << "\r"; */
}
