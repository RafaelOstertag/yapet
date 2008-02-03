// $Id$

#include <string.h>

#include <iostream>

#include <key.h>
#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <partdec.h>
#include <file.h>

#define NAME "Test name"
#define HOST "Test host"
#define UNAME "Test username"
#define PW "Test password"
#define COMMENT "Test comment"

GPSAFE::Record<GPSAFE::PasswordRecord> record1;
GPSAFE::Record<GPSAFE::PasswordRecord> record2;
GPSAFE::Record<GPSAFE::PasswordRecord> record3;
GPSAFE::Record<GPSAFE::PasswordRecord> record4;
GPSAFE::Record<GPSAFE::PasswordRecord> record5;
GPSAFE::Record<GPSAFE::PasswordRecord> record6;
GPSAFE::Record<GPSAFE::PasswordRecord> record7;
GPSAFE::Record<GPSAFE::PasswordRecord> record8;
GPSAFE::Record<GPSAFE::PasswordRecord> record9;
GPSAFE::Record<GPSAFE::PasswordRecord> record10;
GPSAFE::Record<GPSAFE::PasswordRecord> record11;
GPSAFE::Record<GPSAFE::PasswordRecord> record12;
GPSAFE::Record<GPSAFE::PasswordRecord> record13;

void init_records() {
    GPSAFE::PasswordRecord* ptr_rec = record1;
    memcpy(ptr_rec->name, NAME "1", strlen(NAME "1"));
    memcpy(ptr_rec->host, HOST "1", strlen(HOST "1"));
    memcpy(ptr_rec->username, UNAME "1", strlen(UNAME "1"));
    memcpy(ptr_rec->password, PW "1", strlen(PW "1"));
    memcpy(ptr_rec->comment, COMMENT "1", strlen(COMMENT "1"));

    ptr_rec = record2;
    memcpy(ptr_rec->name, NAME "2", strlen(NAME "2"));
    memcpy(ptr_rec->host, HOST "2", strlen(HOST "2"));
    memcpy(ptr_rec->username, UNAME "2", strlen(UNAME "2"));
    memcpy(ptr_rec->password, PW "2", strlen(PW "2"));
    memcpy(ptr_rec->comment, COMMENT "2", strlen(COMMENT "2"));

    ptr_rec = record3;
    memcpy(ptr_rec->name, NAME "3", strlen(NAME "3"));
    memcpy(ptr_rec->host, HOST "3", strlen(HOST "3"));
    memcpy(ptr_rec->username, UNAME "3", strlen(UNAME "3"));
    memcpy(ptr_rec->password, PW "3", strlen(PW "3"));
    memcpy(ptr_rec->comment, COMMENT "3", strlen(COMMENT "3"));

    ptr_rec = record4;
    memcpy(ptr_rec->name, NAME "4", strlen(NAME "4"));
    memcpy(ptr_rec->host, HOST "4", strlen(HOST "4"));
    memcpy(ptr_rec->username, UNAME "4", strlen(UNAME "4"));
    memcpy(ptr_rec->password, PW "4", strlen(PW "4"));
    memcpy(ptr_rec->comment, COMMENT "4", strlen(COMMENT "4"));

    ptr_rec = record5;
    memcpy(ptr_rec->name, NAME "5", strlen(NAME "5"));
    memcpy(ptr_rec->host, HOST "5", strlen(HOST "5"));
    memcpy(ptr_rec->username, UNAME "5", strlen(UNAME "5"));
    memcpy(ptr_rec->password, PW "5", strlen(PW "5"));
    memcpy(ptr_rec->comment, COMMENT "5", strlen(COMMENT "5"));

    ptr_rec = record6;
    memcpy(ptr_rec->name, NAME "6", strlen(NAME "6"));
    memcpy(ptr_rec->host, HOST "6", strlen(HOST "6"));
    memcpy(ptr_rec->username, UNAME "6", strlen(UNAME "6"));
    memcpy(ptr_rec->password, PW "6", strlen(PW "6"));
    memcpy(ptr_rec->comment, COMMENT "6", strlen(COMMENT "6"));

    ptr_rec = record7;
    memcpy(ptr_rec->name, NAME "7", strlen(NAME "7"));
    memcpy(ptr_rec->host, HOST "7", strlen(HOST "7"));
    memcpy(ptr_rec->username, UNAME "7", strlen(UNAME "7"));
    memcpy(ptr_rec->password, PW "7", strlen(PW "7"));
    memcpy(ptr_rec->comment, COMMENT "7", strlen(COMMENT "7"));

    ptr_rec = record8;
    memcpy(ptr_rec->name, NAME "8", strlen(NAME "8"));
    memcpy(ptr_rec->host, HOST "8", strlen(HOST "8"));
    memcpy(ptr_rec->username, UNAME "8", strlen(UNAME "8"));
    memcpy(ptr_rec->password, PW "8", strlen(PW "8"));
    memcpy(ptr_rec->comment, COMMENT "8", strlen(COMMENT "8"));

    ptr_rec = record9;
    memcpy(ptr_rec->name, NAME "9", strlen(NAME "9"));
    memcpy(ptr_rec->host, HOST "9", strlen(HOST "9"));
    memcpy(ptr_rec->username, UNAME "9", strlen(UNAME "9"));
    memcpy(ptr_rec->password, PW "9", strlen(PW "9"));
    memcpy(ptr_rec->comment, COMMENT "9", strlen(COMMENT "9"));

    ptr_rec = record10;
    memcpy(ptr_rec->name, NAME "10", strlen(NAME "10"));
    memcpy(ptr_rec->host, HOST "10", strlen(HOST "10"));
    memcpy(ptr_rec->username, UNAME "10", strlen(UNAME "10"));
    memcpy(ptr_rec->password, PW "10", strlen(PW "10"));
    memcpy(ptr_rec->comment, COMMENT "10", strlen(COMMENT "10"));

    ptr_rec = record11;
    memcpy(ptr_rec->name, NAME "11", strlen(NAME "11"));
    memcpy(ptr_rec->host, HOST "11", strlen(HOST "11"));
    memcpy(ptr_rec->username, UNAME "11", strlen(UNAME "11"));
    memcpy(ptr_rec->password, PW "11", strlen(PW "11"));
    memcpy(ptr_rec->comment, COMMENT "11", strlen(COMMENT "11"));

    ptr_rec = record12;
    memcpy(ptr_rec->name, NAME "12", strlen(NAME "12"));
    memcpy(ptr_rec->host, HOST "12", strlen(HOST "12"));
    memcpy(ptr_rec->username, UNAME "12", strlen(UNAME "12"));
    memcpy(ptr_rec->password, PW "12", strlen(PW "12"));
    memcpy(ptr_rec->comment, COMMENT "12", strlen(COMMENT "12"));

    ptr_rec = record13;
    memcpy(ptr_rec->name, NAME "13", strlen(NAME "13"));
    memcpy(ptr_rec->host, HOST "13", strlen(HOST "13"));
    memcpy(ptr_rec->username, UNAME "13", strlen(UNAME "13"));
    memcpy(ptr_rec->password, PW "13", strlen(PW "13"));
    memcpy(ptr_rec->comment, COMMENT "13", strlen(COMMENT "13"));
}

void print_record(GPSAFE::PartDec& pd, const GPSAFE::Key& key) {
    GPSAFE::Crypt crypt(key);

    std::cout << "PartDec Name:\t" << pd.getName() << std::endl;
    
    const GPSAFE::BDBuffer& enc_rec = pd.getEncRecord();
    GPSAFE::Record<GPSAFE::PasswordRecord>* ptr_dec_rec = crypt.decrypt<GPSAFE::PasswordRecord>(enc_rec);
    GPSAFE::PasswordRecord* ptr_pw = *ptr_dec_rec;

    std::cout << "\tName:\t" << ptr_pw->name << std::endl;
    std::cout << "\tHost:\t" << ptr_pw->host << std::endl;
    std::cout << "\tUname:\t" << ptr_pw->username << std::endl;
    std::cout << "\tPW:\t" << ptr_pw->password << std::endl;
    std::cout << "\tCMT:\t" << ptr_pw->comment << std::endl;
    std::cout << std::endl;

    delete ptr_dec_rec;
}

#define FN "encryptiontest.gps"
int main(int, char**) {
    init_records();

    try {
	GPSAFE::Key key("JustAPassword");
	GPSAFE::File file(FN, key, true);
	std::list<GPSAFE::PartDec> list;
	
	list.push_back(GPSAFE::PartDec(record1, key));
	list.push_back(GPSAFE::PartDec(record2, key));
	list.push_back(GPSAFE::PartDec(record3, key));
	list.push_back(GPSAFE::PartDec(record4, key));
	list.push_back(GPSAFE::PartDec(record5, key));
	list.push_back(GPSAFE::PartDec(record6, key));
	list.push_back(GPSAFE::PartDec(record7, key));
	list.push_back(GPSAFE::PartDec(record8, key));
	list.push_back(GPSAFE::PartDec(record9, key));
	list.push_back(GPSAFE::PartDec(record10, key));
	list.push_back(GPSAFE::PartDec(record11, key));
	list.push_back(GPSAFE::PartDec(record12, key));
	list.push_back(GPSAFE::PartDec(record13, key));
	file.save(list);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    try {
	GPSAFE::Key key("JustAPassword");
	GPSAFE::File file(FN, key, false);
	std::list<GPSAFE::PartDec> list = file.read(key);

	std::list<GPSAFE::PartDec>::iterator it = list.begin();

	while(it != list.end()) {
	    print_record(*it, key);
	    it++;
	}
	
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
