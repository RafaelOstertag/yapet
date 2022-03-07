// $Id$
//
// Check the file written by YAPET using the libpwrecord.so preload
// library

#include <cstdlib>
#include <sstream>

#include "cryptofactoryhelper.hh"
#include "file.hh"

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

constexpr auto TEST_FILE{"/tmp/masterpwchange.pet"};
std::unique_ptr<yapet::Crypto> crypto;

bool operator==(const yapet::PasswordListItem& a, const control_struct& b) {
    std::string name{reinterpret_cast<const char*>(a.name())};

    auto serializedPasswordRecord{crypto->decrypt(a.encryptedRecord())};
    yapet::PasswordRecord passwordRecord{serializedPasswordRecord};

    if (name != b.name) goto OUTNE;

    if (reinterpret_cast<const char*>(passwordRecord.name()) != b.name)
        goto OUTNE;
    if (reinterpret_cast<const char*>(passwordRecord.host()) != b.host)
        goto OUTNE;
    if (reinterpret_cast<const char*>(passwordRecord.username()) != b.user)
        goto OUTNE;
    if (reinterpret_cast<const char*>(passwordRecord.password()) != b.pass)
        goto OUTNE;
    if (reinterpret_cast<const char*>(passwordRecord.comment()) != b.comm)
        goto OUTNE;

    return true;

OUTNE:
    return false;
}

bool operator<(const control_struct& a, const control_struct& b) {
    return a.name < b.name;
}

void init_control_data() {
    std::ostringstream no;

    for (int i = 0; i < 100; i++) {
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

    auto password{yapet::toSecureArray("pleasechange09")};
    std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
        yapet::getCryptoFactoryForFile(TEST_FILE, password)};
    crypto = cryptoFactory->crypto();

    YAPET::File testfile(cryptoFactory, TEST_FILE, false, false);

    std::list<yapet::PasswordListItem> lst = testfile.read();

    if (control_data.size() != lst.size()) abort();

    if (!std::equal(lst.begin(), lst.end(), control_data.begin())) abort();

    std::exit(0);
}
