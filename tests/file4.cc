// $Id$
/*
 * Depends on the file created by test file3
 */

#include <string.h>

#include <iostream>

#include <key.h>
#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <partdec.h>
#include <file.h>

void print_record(GPSAFE::PartDec& pd, const GPSAFE::Key& key, const uint8_t* iv) {
    GPSAFE::Crypt crypt(iv, GPSAFE::IV_SIZE);

    std::cout << "PartDec Name:\t" << pd.getName() << std::endl;
    
    const GPSAFE::BDBuffer& enc_rec = pd.getEncRecord();
    GPSAFE::Record<GPSAFE::PasswordRecord>* ptr_dec_rec = crypt.decrypt<GPSAFE::PasswordRecord>(enc_rec, key);
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
    try {
	GPSAFE::Key key("JustAPassword");
	GPSAFE::File file(FN, key, false);
	std::list<GPSAFE::PartDec> list = file.read(key);

	std::list<GPSAFE::PartDec>::iterator it = list.begin();

	while(it != list.end()) {
	    print_record(*it, key, file.getStoredIV());
	    it++;
	}
	
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
