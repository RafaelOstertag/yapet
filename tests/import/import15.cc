// $Id: import14.cc 5623 2013-11-21 14:36:29Z rafisol $
//
// Test

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <typeinfo>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "csvimport.h"
#include "testpaths.h"
#include <crypt.h>
#include <file.h>

const char* VAL128 = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefg";

const char* VAL256 = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmno";

const char* VAL512 = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789abcdefghijklmnopqrstuvxyzABCDEF";

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;

    try {
	std::cout << " ==> Importing from test10.csv" << std::endl;
	std::cout << " ==> Testing import of oversized lines" << std::endl;
	CSVImport imp ( SRCDIR "/test10.csv", BUILDDIR "/test10.pet", ';');
	imp.import ("test10");

	if (!imp.hadErrors() ) {
	    imp.printLog();
	    return 1;
	}

	if (imp.numErrors() != 3) {
	    std::cerr << "Expected 3 errors, got " << imp.numErrors() << std::endl;
	    imp.printLog();
	    return 1;
	}

	imp.printLog();
    } catch (std::exception& ex) {
	std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	return 1;
    }

    try {
	YAPET::Key key ("test10");
	YAPET::Crypt yacrypt(key);
	YAPET::File file ( BUILDDIR "/test10.pet", key, false);
	assert(file.getFileVersion(key) == YAPET::VERSION_2);
	std::list<YAPET::PartDec> list = file.read (key);

	if (list.size() != 4) {
	    std::cerr << "List size expected to be 3, got " << list.size() << std::endl;
	    return 1;
	}

	std::list<YAPET::PartDec>::iterator it = list.begin();

	//
	// Check first record
	//

	const YAPET::BDBuffer& enc_rec1 = it->getEncRecord();
	YAPET::Record<YAPET::PasswordRecord>* ptr_dec_rec = yacrypt.decrypt<YAPET::PasswordRecord> (enc_rec1);
	YAPET::PasswordRecord* ptr_pw = *ptr_dec_rec;

	if (std::strcmp((char*) ptr_pw->name, VAL128) != 0) {
	    std::cerr << "Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->host, VAL256) != 0) {
	    std::cerr << "Host does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->username, VAL256) != 0) {
	    std::cerr << "User Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->password, VAL256) != 0) {
	    std::cerr << "Password does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->comment, VAL512) != 0) {
	    std::cerr << "Comment does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	delete ptr_dec_rec;

	//
	// Check second record
	//

	it++;
	const YAPET::BDBuffer& enc_rec2 = it->getEncRecord();
	ptr_dec_rec = yacrypt.decrypt<YAPET::PasswordRecord> (enc_rec2);
	ptr_pw = *ptr_dec_rec;

	if (std::strcmp((char*) ptr_pw->name, "name1") != 0) {
	    std::cerr << "Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->host, "host1") != 0) {
	    std::cerr << "Host does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->username, "username1") != 0) {
	    std::cerr << "User Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->password, "password1") != 0) {
	    std::cerr << "Password does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->comment, "comment1") != 0) {
	    std::cerr << "Comment does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	delete ptr_dec_rec;

	//
	// Check third record
	//

	it++;
	const YAPET::BDBuffer& enc_rec3 = it->getEncRecord();
	ptr_dec_rec = yacrypt.decrypt<YAPET::PasswordRecord> (enc_rec3);
	ptr_pw = *ptr_dec_rec;

	if (std::strcmp((char*) ptr_pw->name, "name2") != 0) {
	    std::cerr << "Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->host, "host2") != 0) {
	    std::cerr << "Host does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->username, "username2") != 0) {
	    std::cerr << "User Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->password, "password2") != 0) {
	    std::cerr << "Password does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->comment, "comment2") != 0) {
	    std::cerr << "Comment does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	delete ptr_dec_rec;

	//
	// Check fourth record
	//

	it++;
	const YAPET::BDBuffer& enc_rec4 = it->getEncRecord();
	ptr_dec_rec = yacrypt.decrypt<YAPET::PasswordRecord> (enc_rec4);
	ptr_pw = *ptr_dec_rec;

	if (std::strcmp((char*) ptr_pw->name, "name3") != 0) {
	    std::cerr << "Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->host, "host3") != 0) {
	    std::cerr << "Host does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->username, "username3") != 0) {
	    std::cerr << "User Name does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->password, "password3") != 0) {
	    std::cerr << "Password does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	if (std::strcmp((char*) ptr_pw->comment, "comment3") != 0) {
	    std::cerr << "Comment does not match" << std::endl;
	    delete ptr_dec_rec;
	    return 1;
	}

	delete ptr_dec_rec;

    } catch (std::exception& ex) {
	std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	return 1;
    }

    return 0;

}
