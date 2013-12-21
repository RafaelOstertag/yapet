// $Id$
//
// Check the file written by YAPET using the libpwrecord.so preload
// library

#include <cstdlib>
#include <sstream>
#include "crypt.h"
#include "file.h"

int main() {
    std::string name("Test Name ");
    std::string host("Test Host ");
    std::string user("Test User ");
    std::string pass("Test Password ");
    std::string comm("Test Comment ");
    
    
    YAPET::Key key("pleasechange");
    YAPET::Crypt crypt(key);
    YAPET::File testfile("/tmp/testpwrecord.pet", key, false, false);
    
    std::list<YAPET::PartDec> lst = testfile.read(key);
    
    std::list<YAPET::PartDec>::iterator it = lst.begin();

    std::list<YAPET::PartDec>::size_type c=0;
    while (it!=lst.end()) {

	    YAPET::Record<YAPET::PasswordRecord>* dec_rec = 
		crypt.decrypt<YAPET::PasswordRecord>(it->getEncRecord() );
	    YAPET::PasswordRecord* ptr_rec = *dec_rec;

	    std::ostringstream no;
	    no.str("");
	    no.clear();
	    no << c;

	    if ((char*)ptr_rec->name != (name + no.str()))
		abort();

	    delete dec_rec;

	    c++;
	    it++;
    }

    if (c!=lst.size()-1)
	abort();

    std::exit(0);
}
