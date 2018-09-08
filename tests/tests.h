// -*- c++ -*-
//
// $Id$
//
// This file holds defines common to all tests
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "abstractcryptofactory.hh"
#include "crypt.h"
#include "passwordlistitem.hh"

#ifndef ROUNDS
#define ROUNDS 5000
#endif

#define FN "encryptiontest.gps"

#ifndef NAME
#define NAME "Test name %d"
#endif

#ifndef HOST
#define HOST "Test host %d"
#endif

#ifndef UNAME
#define UNAME "Test username %d"
#endif

#ifndef PW
#define PW "Test password %d"
#endif

#ifndef COMMENT
#define COMMENT "Test comment %d"
#endif

inline void progress() {
    static int n = 0;

    // I change the progress indicator to somewhat less verbose, since the
    // above indicator looks terrible in build logs of automated builds,
    // e.g. :
    // https://buildd.debian.org/pkg.cgi?pkg=yapet
    if ((n % 100) == 0 && !((n % 1000) == 0)) {
        std::cout << ".";
        std::cout.flush();
    }

    if ((n % 1000) == 0 && n != 0) {
        std::cout << "#";
        std::cout.flush();
    }

    n++;
}

inline void print_record(
    const yapet::PasswordListItem& passwordListItem,
    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory) {
    auto crypto{cryptoFactory->crypto()};

    std::cout << "PasswordListItemName Name:\t" << passwordListItem.name()
              << std::endl;

    yapet::PasswordRecord passwordRecord{
        crypto->decrypt(passwordListItem.encryptedRecord())};

    std::cout << "\tName:\t" << passwordRecord.name() << std::endl;
    std::cout << "\tHost:\t" << passwordRecord.host() << std::endl;
    std::cout << "\tUname:\t" << passwordRecord.username() << std::endl;
    std::cout << "\tPW:\t" << passwordRecord.password() << std::endl;
    std::cout << "\tCMT:\t" << passwordRecord.comment() << std::endl;
    std::cout << std::endl;
}

inline void check_record(
    const yapet::PasswordListItem& passwordListItem,
    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory, int rec_no) {
    progress();

    auto crypto{cryptoFactory->crypto()};

    char _name[yapet::PasswordRecord::NAME_SIZE];
    char _host[yapet::PasswordRecord::HOST_SIZE];
    char _uname[yapet::PasswordRecord::USERNAME_SIZE];
    char _pw[yapet::PasswordRecord::PASSWORD_SIZE];
    char _cmt[yapet::PasswordRecord::COMMENT_SIZE];
    snprintf(_name, yapet::PasswordRecord::NAME_SIZE, NAME, rec_no);
    snprintf(_host, yapet::PasswordRecord::HOST_SIZE, HOST, rec_no);
    snprintf(_uname, yapet::PasswordRecord::USERNAME_SIZE, UNAME, rec_no);
    snprintf(_pw, yapet::PasswordRecord::PASSWORD_SIZE, PW, rec_no);
    snprintf(_cmt, yapet::PasswordRecord::COMMENT_SIZE, COMMENT, rec_no);

    yapet::PasswordRecord passwordRecord{
        crypto->decrypt(passwordListItem.encryptedRecord())};

    if (std::strncmp(_name,
                     reinterpret_cast<const char*>(passwordListItem.name()),
                     yapet::PasswordRecord::NAME_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("Name does not match");
    }

    if (std::strncmp(_name,
                     reinterpret_cast<const char*>(passwordRecord.name()),
                     yapet::PasswordRecord::NAME_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("Name does not match");
    }

    if (std::strncmp(_host,
                     reinterpret_cast<const char*>(passwordRecord.host()),
                     yapet::PasswordRecord::HOST_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("Host does not match");
    }

    if (std::strncmp(_uname,
                     reinterpret_cast<const char*>(passwordRecord.username()),
                     yapet::PasswordRecord::USERNAME_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("username does not match");
    }

    if (std::strncmp(_pw,
                     reinterpret_cast<const char*>(passwordRecord.password()),
                     yapet::PasswordRecord::PASSWORD_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("password does not match");
    }

    if (std::strncmp(_cmt,
                     reinterpret_cast<const char*>(passwordRecord.comment()),
                     yapet::PasswordRecord::COMMENT_SIZE) != 0) {
        print_record(passwordListItem, cryptoFactory);
        throw std::logic_error("comment does not match");
    }
}
