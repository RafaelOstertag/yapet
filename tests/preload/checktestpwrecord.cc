// $Id$
//
// Check the file written by YAPET using the libpwrecord.so preload
// library

#include <cstdlib>
#include <sstream>
#include "crypt.h"
#include "file.h"

struct control_struct {
	std::string name;
	std::string host;
	std::string user;
	std::string pass;
	std::string comm;
};

std::string name("Test Name ");
std::string host("Test Host ");
std::string user("Test User ");
std::string pass("Test Password ");
std::string comm("Test Comment ");

std::list<control_struct> control_data;
YAPET::Key key("pleasechange");
YAPET::Crypt yacrypt(key);
YAPET::File testfile("/tmp/testpwrecord.pet", key, false, false);

bool operator==(const YAPET::PartDec& a, const control_struct& b) {
    YAPET::Record<YAPET::PasswordRecord>* dec_rec = 
	yacrypt.decrypt<YAPET::PasswordRecord>(a.getEncRecord() );
    YAPET::PasswordRecord* ptr_rec = *dec_rec;
    
    if (reinterpret_cast<const char*>(ptr_rec->name) != b.name)
	goto OUTNE;
    if (reinterpret_cast<const char*>(ptr_rec->host) != b.host)
	goto OUTNE;
    if (reinterpret_cast<const char*>(ptr_rec->username) != b.user)
	goto OUTNE;
    if (reinterpret_cast<const char*>(ptr_rec->password) != b.pass)
	goto OUTNE;
    if (reinterpret_cast<const char*>(ptr_rec->comment) != b.comm)
	goto OUTNE;
	    

    delete dec_rec;
    return true;
    
 OUTNE:
    delete dec_rec;
    return false;
}

bool operator<(const control_struct& a, const control_struct& b) {
    return a.name < b.name;
}

void 
init_control_data() {
    std::ostringstream no;

    for (int i=0; i<100; i++) {
	no.str("");
	no.clear();
	no << i;

	control_struct tmp;
	tmp.name = name + no.str();
	tmp.host = host + no.str();
	tmp.user = user + no.str();
	tmp.pass = pass + no.str();
	tmp.comm = comm + no.str();

	control_data.push_back(tmp);
    }

    control_data.sort();
}

int main() {
    init_control_data();

    std::list<YAPET::PartDec> lst = testfile.read(key);
    
    if (control_data.size() != lst.size())
	abort();

    if (!std::equal(lst.begin(), lst.end(),
		    control_data.begin()))
	abort();

    std::exit(0);
}
