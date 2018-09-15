// $Id: import14.cc 5623 2013-11-21 14:36:29Z rafisol $
//
// Test

#include <iostream>
#include <typeinfo>

#include "aes256factory.hh"
#include "csvimport.h"
#include "file.h"
#include "testpaths.h"

const char* VAL128 =
    "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmn"
    "opqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefg";

const char* VAL256 =
    "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmn"
    "opqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABC"
    "DEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQ"
    "RSTUVXYZ0123456789abcdefghijklmno";

const char* VAL512 =
    "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmn"
    "opqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABC"
    "DEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQ"
    "RSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ012345"
    "6789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghij"
    "klmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxy"
    "zABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEF";

int main(int, char**) {
    try {
        CSVImport imp(SRCDIR "/test10.csv", BUILDDIR "/test10.pet", ';');
        imp.import("test10");

        if (!imp.hadErrors()) {
            imp.printLog();
            return 1;
        }

        if (imp.numErrors() != 3) {
            std::cerr << "Expected 3 errors, got " << imp.numErrors()
                      << std::endl;
            imp.printLog();
            return 1;
        }

        imp.printLog();
    } catch (std::exception& ex) {
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    try {
        auto password{yapet::toSecureArray("test10")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};
        auto crypto{cryptoFactory->crypto()};

        YAPET::File file{cryptoFactory, BUILDDIR "/test10.pet", false};

        assert(file.getHeaderVersion() == yapet::HEADER_VERSION::VERSION_2);
        std::list<yapet::PasswordListItem> list = file.read();

        if (list.size() != 4) {
            std::cerr << "List size expected to be 4, got " << list.size()
                      << std::endl;
            return 1;
        }

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        //
        // Check first record
        //

        yapet::PasswordRecord passwordRecord{
            crypto->decrypt(it->encryptedRecord())};

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.name()),
                        VAL128) != 0) {
            std::cerr << "Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.host()),
                        VAL256) != 0) {
            std::cerr << "Host does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.username()),
                VAL256) != 0) {
            std::cerr << "User Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.password()),
                VAL256) != 0) {
            std::cerr << "Password does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.comment()),
                        VAL512) != 0) {
            std::cerr << "Comment does not match" << std::endl;

            return 1;
        }

        //
        // Check second record
        //

        it++;
        passwordRecord =
            yapet::PasswordRecord{crypto->decrypt(it->encryptedRecord())};

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.name()),
                        "name1") != 0) {
            std::cerr << "Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.host()),
                        "host1") != 0) {
            std::cerr << "Host does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.username()),
                "username1") != 0) {
            std::cerr << "User Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.password()),
                "password1") != 0) {
            std::cerr << "Password does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.comment()),
                        "comment1") != 0) {
            std::cerr << "Comment does not match" << std::endl;

            return 1;
        }

        //
        // Check third record
        //

        it++;
        passwordRecord =
            yapet::PasswordRecord{crypto->decrypt(it->encryptedRecord())};

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.name()),
                        "name2") != 0) {
            std::cerr << "Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.host()),
                        "host2") != 0) {
            std::cerr << "Host does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.username()),
                "username2") != 0) {
            std::cerr << "User Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.password()),
                "password2") != 0) {
            std::cerr << "Password does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.comment()),
                        "comment2") != 0) {
            std::cerr << "Comment does not match" << std::endl;

            return 1;
        }

        //
        // Check fourth record
        //

        it++;
        passwordRecord =
            yapet::PasswordRecord{crypto->decrypt(it->encryptedRecord())};

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.name()),
                        "name3") != 0) {
            std::cerr << "Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.host()),
                        "host3") != 0) {
            std::cerr << "Host does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.username()),
                "username3") != 0) {
            std::cerr << "User Name does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(
                reinterpret_cast<const char*>(passwordRecord.password()),
                "password3") != 0) {
            std::cerr << "Password does not match" << std::endl;

            return 1;
        }

        if (std::strcmp(reinterpret_cast<const char*>(passwordRecord.comment()),
                        "comment3") != 0) {
            std::cerr << "Comment does not match" << std::endl;

            return 1;
        }

    } catch (std::exception& ex) {
        std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
