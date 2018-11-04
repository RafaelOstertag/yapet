#include <iostream>

#include "aes256factory.hh"
#include "cryptoerror.hh"
#include "cryptofactoryhelper.hh"
#include "file.h"
#include "filehelper.hh"
#include "securearray.hh"
#include "testpaths.h"
#include "yapeterror.hh"

constexpr int ROUNDS{10000};
constexpr int NUMBER_OF_PASSWORD_RECORDS{5};
constexpr auto FN{BUILDDIR "/passwordchange_exerciser.pet"};
const std::string PASSWORD_PREFIX{"password"};

constexpr auto NAME{"Name"};
constexpr auto HOST{"Host"};
constexpr auto USERNAME{"Username"};
constexpr auto PASSWORD{"Password"};
constexpr auto COMMENT{"Comment"};

std::string makeName(int number) {
    std::string name{NAME};
    name += " " + std::to_string(number);

    return name;
}

yapet::PasswordRecord makePasswordRecord(int number) {
    yapet::PasswordRecord passwordRecord{};

    passwordRecord.name(makeName(number).c_str());

    std::string host = HOST;
    host += " " + std::to_string(number);
    passwordRecord.host(host.c_str());

    std::string username = USERNAME;
    username += " " + std::to_string(number);
    passwordRecord.username(username.c_str());

    std::string password = PASSWORD;
    password += " " + std::to_string(number);
    passwordRecord.password(password.c_str());

    std::string comment = COMMENT;
    comment += " " + std::to_string(number);
    passwordRecord.comment(comment.c_str());

    return passwordRecord;
}

std::list<yapet::PasswordListItem> createPasswordList(
    std::unique_ptr<yapet::Crypto> &aes256) {
    std::list<yapet::PasswordListItem> passwordList{};
    for (int i = 0; i < NUMBER_OF_PASSWORD_RECORDS; i++) {
        auto passwordRecord{makePasswordRecord(i)};

        auto serializedPasswordRecord{passwordRecord.serialize()};
        auto encryptedSerializedPasswordRecord{
            aes256->encrypt(serializedPasswordRecord)};

        yapet::PasswordListItem passwordListItem{
            makeName(i).c_str(), encryptedSerializedPasswordRecord};

        passwordList.push_back(passwordListItem);
    }
    return passwordList;
}

void progressIndicator(int i) {
    if (i % 100 == 0) {
        std::cerr << (i % 1000 == 0 ? '#' : '.');
    }
}

void comparePasswordRecords(const yapet::PasswordRecord &actual,
                            const yapet::PasswordRecord &expected) {
    assert(std::strcmp(reinterpret_cast<const char *>(actual.name()),
                       reinterpret_cast<const char *>(expected.name())) == 0);
    assert(std::strcmp(reinterpret_cast<const char *>(actual.host()),
                       reinterpret_cast<const char *>(expected.host())) == 0);
    assert(std::strcmp(reinterpret_cast<const char *>(actual.username()),
                       reinterpret_cast<const char *>(expected.username())) ==
           0);
    assert(std::strcmp(reinterpret_cast<const char *>(actual.password()),
                       reinterpret_cast<const char *>(expected.password())) ==
           0);
    assert(std::strcmp(reinterpret_cast<const char *>(actual.comment()),
                       reinterpret_cast<const char *>(expected.comment())) ==
           0);
}

int main() {
    {
        auto password{yapet::toSecureArray(PASSWORD_PREFIX + "0")};
        std::shared_ptr<yapet::Aes256Factory> factory{new yapet::Aes256Factory{
            password, yapet::Key256::newDefaultKeyingParameters()}};

        auto aes256{factory->crypto()};

        YAPET::File file{factory, FN, true};

        auto passwordList{createPasswordList(aes256)};

        file.save(passwordList);
    }

    for (int i = 1; i < ROUNDS; i++) {
        auto previousPassword{
            yapet::toSecureArray(PASSWORD_PREFIX + std::to_string(i - 1))};
        auto currentFactory{
            yapet::getCryptoFactoryForFile(FN, previousPassword)};

        auto newPassword{
            yapet::toSecureArray(PASSWORD_PREFIX + std::to_string(i))};
        std::shared_ptr<yapet::Aes256Factory> newFactory{
            new yapet::Aes256Factory{
                newPassword, yapet::Key256::newDefaultKeyingParameters()}};

        YAPET::File file{currentFactory, FN, false};
        file.setNewKey(newFactory);

        std::list<yapet::PasswordListItem> list = file.read();
        assert(list.size() == NUMBER_OF_PASSWORD_RECORDS);
        progressIndicator(i);
    }

    std::cout << std::endl;
}
