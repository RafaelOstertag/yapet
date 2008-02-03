// $Id$

#include <string.h>

#include <iostream>

#include <key.h>
#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <partdec.h>

#define NAME "Test name"
#define HOST "Test host"
#define UNAME "Test username"
#define PW "Test password"
#define COMMENT "Test comment"

int main(int, char**) {
    try {
	GPSAFE::Key key("TEST");
	GPSAFE::Crypt crypt(key);

	GPSAFE::Record<GPSAFE::PasswordRecord> record;
	GPSAFE::PasswordRecord* ptr_rec = record;
	memcpy(ptr_rec->name, NAME, strlen(NAME));
	memcpy(ptr_rec->host, HOST, strlen(HOST));
	memcpy(ptr_rec->username, UNAME, strlen(UNAME));
	memcpy(ptr_rec->password, PW, strlen(PW));
	memcpy(ptr_rec->comment, COMMENT, strlen(COMMENT));

	GPSAFE::PartDec pdec;
	pdec.setRecord(record, key);

	GPSAFE::Record<GPSAFE::PasswordRecord>* dec_rec = crypt.decrypt<GPSAFE::PasswordRecord>(pdec.getEncRecord());
	GPSAFE::PasswordRecord* ptr_dec_rec = *dec_rec;

	int retval = memcmp(ptr_rec->name, ptr_dec_rec->name, GPSAFE::NAME_SIZE);
	if (retval != 0)
	    return 1;
	retval = memcmp(ptr_rec->host, ptr_dec_rec->host, GPSAFE::HOST_SIZE);
	if (retval != 0)
	    return 1;
	retval = memcmp(ptr_rec->username, ptr_dec_rec->username, GPSAFE::USERNAME_SIZE);
	if (retval != 0)
	    return 1;
	retval = memcmp(ptr_rec->password, ptr_dec_rec->password, GPSAFE::PASSWORD_SIZE);
	if (retval != 0)
	    return 1;
	retval = memcmp(ptr_rec->comment, ptr_dec_rec->comment, GPSAFE::COMMENT_SIZE);
	if (retval != 0)
	    return 1;

	delete dec_rec;
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    return 0;
}
