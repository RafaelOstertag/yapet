// $Id$


#include <string.h>

#include <file.h>

#include "tests.h"

int main(int, char**) {
    std::cout << "Be patient, this test may take a few minutes ..." << std::endl;
    try {
	YAPET::Key key("JustAPassword");
	YAPET::File file(FN, key, true);
	std::list<YAPET::PartDec> list;

	for (int i=0; i < ROUNDS; i++) {
	    progress();
	    YAPET::Record<YAPET::PasswordRecord> record;
	    YAPET::PasswordRecord *ptr_rec = record;
	    snprintf((char*)ptr_rec->name, YAPET::NAME_SIZE, NAME, i);
	    snprintf((char*)ptr_rec->host, YAPET::HOST_SIZE, HOST, i);
	    snprintf((char*)ptr_rec->username, YAPET::USERNAME_SIZE, UNAME, i);
	    snprintf((char*)ptr_rec->password,YAPET::PASSWORD_SIZE, PW, i);
	    snprintf((char*)ptr_rec->comment,YAPET::COMMENT_SIZE, COMMENT,i);
	    list.push_back(YAPET::PartDec(record, key));
	    
	    std::cout << "\r";
	}
	file.save(list);
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    try {
	YAPET::Key key("JustAPassword");
	YAPET::File file(FN, key, false);
	std::list<YAPET::PartDec> list = file.read(key);
	if (list.size() != ROUNDS)
	    return 1;


	std::list<YAPET::PartDec>::iterator it = list.begin();

	for(int i=0; it != list.end(); i++) {
	    check_record(*it, key, i);
	    it++;
	}
	
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;
}
