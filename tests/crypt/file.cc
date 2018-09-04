#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <unistd.h>
#include <cstring>
#include <list>

#include "blowfishfactory.hh"
#include "file.h"
#include "securearray.hh"
#include "testpaths.h"

constexpr auto TEST_PASSWORD{"Secret"};

constexpr auto FN{BUILDDIR "/testfile.gps"};
constexpr auto ROUNDS{10};

constexpr auto NAME{"Name"};
constexpr auto HOST{"Host"};
constexpr auto USERNAME{"Username"};
constexpr auto PASSWORD{"Password"};
constexpr auto COMMENT{"Comment"};

inline std::string makeName(int number) {
    std::string name{NAME};
    name += " " + std::to_string(number);

    return name;
}

inline yapet::PasswordRecord makePasswordRecord(int number) {
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

inline std::list<yapet::PasswordListItem> createPasswordList(
    std::shared_ptr<yapet::Crypto> blowfish) {
    std::list<yapet::PasswordListItem> passwordList{};
    for (int i = 0; i < ROUNDS; i++) {
        auto passwordRecord{makePasswordRecord(i)};

        auto serializedPasswordRecord{passwordRecord.serialize()};
        auto encryptedSerializedPasswordRecord{
            blowfish->encrypt(serializedPasswordRecord)};

        yapet::PasswordListItem passwordListItem{
            makeName(i).c_str(), encryptedSerializedPasswordRecord};

        passwordList.push_back(passwordListItem);
    }
    return passwordList;
}

inline void comparePasswordRecords(const yapet::PasswordRecord &actual,
                                   const yapet::PasswordRecord &expected) {
    CPPUNIT_ASSERT(
        std::strcmp(reinterpret_cast<const char *>(actual.name()),
                    reinterpret_cast<const char *>(expected.name())) == 0);
    CPPUNIT_ASSERT(
        std::strcmp(reinterpret_cast<const char *>(actual.host()),
                    reinterpret_cast<const char *>(expected.host())) == 0);
    CPPUNIT_ASSERT(
        std::strcmp(reinterpret_cast<const char *>(actual.username()),
                    reinterpret_cast<const char *>(expected.username())) == 0);
    CPPUNIT_ASSERT(
        std::strcmp(reinterpret_cast<const char *>(actual.password()),
                    reinterpret_cast<const char *>(expected.password())) == 0);
    CPPUNIT_ASSERT(
        std::strcmp(reinterpret_cast<const char *>(actual.comment()),
                    reinterpret_cast<const char *>(expected.comment())) == 0);
}

class FileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Blowfish");

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should correctly read empty file", &FileTest::createNewFile));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should correctly read empty file",
            &FileTest::createAndReadEmptyFile));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should throw expected exception on invalid password",
            &FileTest::openEmptyFileWithInvalidPassword));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should write passwords", &FileTest::writePasswords));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should detect file modification on password save",
            &FileTest::detectModificationOnSave));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should force password save on modified file",
            &FileTest::forceSave));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should get the correct time when the master password was set",
            &FileTest::timeMasterPasswordSet));
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should properly set new password", &FileTest::setNewPassword));
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should allow saving passwords after password change",
            &FileTest::allowSaveAfterPasswordSave));

        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>(
            "should throw exception on reading corrupt file",
            &FileTest::corruptFile));

        return suiteOfTests;
    }

    void setUp() { unlink(FN); }

    void tearDown() { unlink(FN); }

    void createNewFile() {
        auto password{yapet::toSecureArray(TEST_PASSWORD)};
        std::shared_ptr<yapet::BlowfishFactory> factory{
            new yapet::BlowfishFactory{}};

        YAPET::File file{factory, FN, password, true};
        auto expectedFileVersion{yapet::toSecureArray("YAPET1.0")};

        CPPUNIT_ASSERT(std::memcmp(*file.getFileVersion(), *expectedFileVersion,
                                   expectedFileVersion.size()) == 0);
        CPPUNIT_ASSERT(file.getHeaderVersion() ==
                       yapet::HEADER_VERSION::VERSION_2);
    }

    void createAndReadEmptyFile() {
        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            YAPET::File file{factory, FN, password, true};
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            YAPET::File file{factory, FN, password, false};
            auto expectedFileVersion{yapet::toSecureArray("YAPET1.0")};

            CPPUNIT_ASSERT(std::memcmp(*file.getFileVersion(),
                                       *expectedFileVersion,
                                       expectedFileVersion.size()) == 0);
            CPPUNIT_ASSERT(file.getHeaderVersion() ==
                           yapet::HEADER_VERSION::VERSION_2);
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }
    }

    void openEmptyFileWithInvalidPassword() {
        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            YAPET::File file{factory, FN, password, true};
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray("InvalidPassword")};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            CPPUNIT_ASSERT_THROW((YAPET::File{factory, FN, password, false}),
                                 YAPET::YAPETInvalidPasswordException);
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }
    }

    void writePasswords() {
        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            auto blowfish{factory->crypto(password)};

            YAPET::File file{factory, FN, password, true};

            auto passwordList{createPasswordList(blowfish)};

            file.save(passwordList);
        } catch (std::exception &ex) {
            std::cout << std::endl;
            std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;

            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            auto blowfish{factory->crypto(password)};

            YAPET::File file{factory, FN, password, false};

            std::list<yapet::PasswordListItem> list = file.read();
            CPPUNIT_ASSERT(list.size() == ROUNDS);

            std::list<yapet::PasswordListItem>::iterator it = list.begin();

            for (int i = 0; it != list.end(); i++, it++) {
                auto expectedPasswordRecord{makePasswordRecord(i)};
                CPPUNIT_ASSERT(
                    std::strcmp(reinterpret_cast<const char *>(it->name()),
                                reinterpret_cast<const char *>(
                                    expectedPasswordRecord.name())) == 0);

                auto decryptedSerializedPasswordRecord{
                    blowfish->decrypt(it->encryptedRecord())};

                yapet::PasswordRecord actual{decryptedSerializedPasswordRecord};
                comparePasswordRecords(actual, expectedPasswordRecord);
            }
        } catch (std::exception &ex) {
            std::cout << std::endl;
            std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
            CPPUNIT_FAIL("unexpected exception");
        }
    }

    void detectModificationOnSave() {
        auto password{yapet::toSecureArray(TEST_PASSWORD)};
        std::shared_ptr<yapet::BlowfishFactory> factory{
            new yapet::BlowfishFactory{}};

        YAPET::File file1{factory, FN, password, true};
        YAPET::File file2{factory, FN, password, false};

        std::list<yapet::PasswordListItem> passwordList{};
        auto passwordRecord{makePasswordRecord(1)};

        auto serializedPasswordRecord{passwordRecord.serialize()};

        auto blowfish{factory->crypto(password)};
        auto encryptedSerializedPasswordRecord{
            blowfish->encrypt(serializedPasswordRecord)};

        yapet::PasswordListItem passwordListItem{
            makeName(1).c_str(), encryptedSerializedPasswordRecord};

        passwordList.push_back(passwordListItem);

        // we're using second resolution, thus sleep one second in order to
        // allow the update to be detected
        ::sleep(1);

        // This modifies the mtime of the file
        file1.save(passwordList);

        // the modification of the file's mtime should be detected here
        CPPUNIT_ASSERT_THROW(file2.save(passwordList),
                             YAPET::YAPETRetryException);

        ::sleep(1);
        // file1 must allow to re-save
        file1.save(passwordList);
    }

    void forceSave() {
        auto password{yapet::toSecureArray(TEST_PASSWORD)};
        std::shared_ptr<yapet::BlowfishFactory> factory{
            new yapet::BlowfishFactory{}};

        YAPET::File file1{factory, FN, password, true};
        YAPET::File file2{factory, FN, password, false};

        std::list<yapet::PasswordListItem> passwordList{};
        auto passwordRecord{makePasswordRecord(1)};
        auto serializedPasswordRecord{passwordRecord.serialize()};
        auto blowfish{factory->crypto(password)};
        auto encryptedSerializedPasswordRecord{
            blowfish->encrypt(serializedPasswordRecord)};

        yapet::PasswordListItem passwordListItem{
            makeName(1).c_str(), encryptedSerializedPasswordRecord};

        passwordList.push_back(passwordListItem);

        // we're using second resolution, thus sleep one second in order to
        // allow the update to be detected
        ::sleep(1);

        // This modifies the mtime of the file
        file1.save(passwordList);

        passwordRecord = makePasswordRecord(2);
        serializedPasswordRecord = passwordRecord.serialize();
        encryptedSerializedPasswordRecord =
            blowfish->encrypt(serializedPasswordRecord);

        yapet::PasswordListItem passwordListItem2{
            makeName(2).c_str(), encryptedSerializedPasswordRecord};

        std::list<yapet::PasswordListItem> passwordList2{};
        passwordList2.push_back(passwordListItem2);

        // the modification of the file's mtime should be detected here
        CPPUNIT_ASSERT_THROW(file2.save(passwordList2),
                             YAPET::YAPETRetryException);
        file2.save(passwordList2, true);

        auto actual{file2.read()};

        CPPUNIT_ASSERT(actual.size() == 1);

        auto serializedEncryptedRecord{actual.begin()->encryptedRecord()};
        auto serializedRecord{blowfish->decrypt(serializedEncryptedRecord)};
        yapet::PasswordRecord actualPasswordRecord{serializedRecord};

        comparePasswordRecords(actualPasswordRecord, passwordRecord);
    }

    void timeMasterPasswordSet() {
        std::time_t approxTimePasswordSet;
        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            approxTimePasswordSet = std::time(0);
            YAPET::File file{factory, FN, password, true};
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            YAPET::File file{factory, FN, password, false};
            int64_t passwordSet{file.getMasterPWSet()};

            CPPUNIT_ASSERT(((approxTimePasswordSet - 10) < passwordSet) &&
                           ((approxTimePasswordSet + 10) > passwordSet));
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }
    }

    void setNewPassword() {
        try {
            auto password{yapet::toSecureArray(TEST_PASSWORD)};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            auto blowfish{factory->crypto(password)};

            YAPET::File file{factory, FN, password, true};

            auto passwordList{createPasswordList(blowfish)};

            file.save(passwordList);

            auto newPassword{yapet::toSecureArray("NewSecret")};
            file.setNewKey(newPassword);
        } catch (std::exception &ex) {
            std::cout << std::endl;
            std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;

            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray("NewSecret")};
            std::shared_ptr<yapet::BlowfishFactory> factory{
                new yapet::BlowfishFactory{}};

            auto blowfish{factory->crypto(password)};

            YAPET::File file{factory, FN, password, false};

            std::list<yapet::PasswordListItem> list = file.read();
            CPPUNIT_ASSERT(list.size() == ROUNDS);

            std::list<yapet::PasswordListItem>::iterator it = list.begin();

            for (int i = 0; it != list.end(); i++, it++) {
                auto expectedPasswordRecord{makePasswordRecord(i)};
                CPPUNIT_ASSERT(
                    std::strcmp(reinterpret_cast<const char *>(it->name()),
                                reinterpret_cast<const char *>(
                                    expectedPasswordRecord.name())) == 0);

                auto decryptedSerializedPasswordRecord{
                    blowfish->decrypt(it->encryptedRecord())};

                yapet::PasswordRecord actual{decryptedSerializedPasswordRecord};
                comparePasswordRecords(actual, expectedPasswordRecord);
            }
        } catch (std::exception &ex) {
            std::cout << std::endl;
            std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;
            CPPUNIT_FAIL("unexpected exception");
        }
    };

    void allowSaveAfterPasswordSave() {
        auto password{yapet::toSecureArray(TEST_PASSWORD)};
        std::shared_ptr<yapet::BlowfishFactory> factory{
            new yapet::BlowfishFactory{}};

        auto blowfish{factory->crypto(password)};

        YAPET::File file{factory, FN, password, true};

        auto passwordList{createPasswordList(blowfish)};

        file.save(passwordList);

        // Make sure mtime change may be picked up
        ::sleep(1);
        auto newPassword{yapet::toSecureArray("NewSecret")};
        file.setNewKey(newPassword);

        file.save(passwordList);
    }

    void corruptFile() {
        // The file has the byte at offset 0x2e changed from 0x05 to 0x06,
        // messing up the length indicator for the first record
        auto password{yapet::toSecureArray(TEST_PASSWORD)};
        std::shared_ptr<yapet::BlowfishFactory> factory{
            new yapet::BlowfishFactory{}};

        YAPET::File file{factory, BUILDDIR "/corrupt.pet", password, false,
                         false};

        CPPUNIT_ASSERT_THROW(file.read(), YAPET::YAPETEncryptionException);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(FileTest ::suite());
    return runner.run() ? 0 : 1;
}