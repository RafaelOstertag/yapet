// $Id$
//
// Testing whether or not files generated on the author's machine can be read
// on other machines as well...
//
// Relies on foreign.pet

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <typeinfo>
#include <cstring>
#include <iostream>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include "key.h"
#include "crypt.h"
#include "structs.h"
#include "record.h"
#include "partdec.h"
#include "file.h"

#include "tests.h"
#include "testpaths.h"

#ifdef ROUNDS
#undef ROUNDS
#endif
#define ROUNDS 200

int main (int, char**) {
#ifndef TESTS_VERBOSE
    int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
    dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
    std::cout << std::endl;
    std::cout << " ==> Check if we can read files created on a different machine... " << std::endl;

    YAPET::Key* key = NULL;
    YAPET::File *file = NULL;
    int retval = 0;

    //
    //
    // ################# pre 0.6 tests #################
    //
    //


    /*
     * 32bit little endian pre 0.6
     */
    try {
	std::cout << " ==> 32bit little endian pre 0.6 (f32le0.5.pet)" << std::endl;
        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f32le0.5.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_1);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	retval = 1;
    }
    file = NULL;
    key = NULL;

    /*
     * 32bit big endian pre 0.6
     */
    try {
	std::cout << " ==> 32bit big endian pre 0.6 (f32be0.5.pet)" << std::endl;
        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f32be0.5.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_1);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	retval = 1;
    }
    file = NULL;
    key = NULL;

    /*
     * 64bit little endian pre 0.6
     */
    try {
	std::cout << " ==> 64bit little endian pre 0.6 (f64le0.5.pet)" << std::endl;
        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f64le0.5.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_1);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	retval = 1;
    }
    file = NULL;
    key = NULL;
    
    /*
     * 64bit big endian pre 0.6
     */
    try {
	std::cout << " ==> 64bit big endian pre 0.6 (f64be0.5.pet)" << std::endl;

        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f64be0.5.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_1);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;

	retval = 1;
    }
    file = NULL;
    key = NULL;


    //
    //
    // ################# 0.6 tests #################
    //
    //


    /*
     * 32bit little endian 0.6
     */
    try {
	std::cout << " ==> 32bit little endian 0.6 (f32le0.6.pet)" << std::endl;
        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f32le0.6.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_2);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	retval = 1;
    }
    file = NULL;
    key = NULL;

    /*
     * 32bit big endian 0.6
     */
    try {
	std::cout << " ==> 32bit little endian 0.6 (f32be0.6.pet)" << std::endl;
        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f32be0.6.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_2);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;
	retval = 1;
    }
    file = NULL;
    key = NULL;

    /*
     * 64bit little endian 0.6
     */
    try {
	std::cout << " ==> 64bit little endian 0.6 (f64le0.6.pet)" << std::endl;

        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f64le0.6.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_2);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;

	retval = 1;
    }
    file = NULL;
    key = NULL;

    /*
     * 64bit big endian 0.6
     */
    try {
	std::cout << " ==> 64bit big endian 0.6 (f64be0.6.pet)" << std::endl;

        key = new YAPET::Key("test1");
        file = new YAPET::File(BUILDDIR "/f64be0.6.pet", *key, false, false);
	assert(file->getFileVersion(*key) == YAPET::VERSION_2);
        std::list<YAPET::PartDec> list = file->read (*key);

        if (list.size() != ROUNDS) {
            std::cout << std::endl;
            std::cout << " --> no" << std::endl;
            return 1;
        }

        std::list<YAPET::PartDec>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            check_record (*it, *key, i);
            it++;
        }
	delete file;
	delete key;
	std::cout << std::endl;
	std::cout << " --> yes" << std::endl;
    } catch (std::exception& ex) {
	if (file != NULL)
	    delete key;
	if (key != NULL)
	    delete key;
        std::cout << std::endl;
        std::cout << " --> no" << std::endl;
        std::cout << typeid (ex).name() << ": " << ex.what() << std::endl;

	retval = 1;
    }
    file = NULL;
    key = NULL;

    return retval;

}
