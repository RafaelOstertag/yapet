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

class File1Test : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::Blowfish> blowfish;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Blowfish");

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should correctly read empty file", &File1Test::createNewFile));

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should correctly read empty file",
            &File1Test::createAndReadEmptyFile));

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should throw expected exception on invalid password",
            &File1Test::openEmptyFileWithInvalidPassword));

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should write passwords", &File1Test::writePasswords));

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should detect file modification on password save",
            &File1Test::detectModificationOnSave));

        suiteOfTests->addTest(new CppUnit::TestCaller<File1Test>(
            "should force password save on modified file",
            &File1Test::forceSave));

        return suiteOfTests;
    }

    void setUp() { unlink(FN); }

    void tearDown() { unlink(FN); }

    void createNewFile() {
        auto password{
            yapet::toSecureArray("JustATestPasswordForKeepingSecret")};
        yapet::BlowfishFactory factory;

        YAPET::File file{factory, FN, password, true};
        auto expectedFileVersion{yapet::toSecureArray("YAPET1.0")};

        CPPUNIT_ASSERT(std::memcmp(*file.getFileVersion(), *expectedFileVersion,
                                   expectedFileVersion.size()) == 0);
        CPPUNIT_ASSERT(file.getHeaderVersion() ==
                       yapet::HEADER_VERSION::VERSION_2);
    }

    void createAndReadEmptyFile() {
        try {
            auto password{
                yapet::toSecureArray("JustATestPasswordForKeepingSecret")};
            yapet::BlowfishFactory factory;

            YAPET::File file{factory, FN, password, true};
            // avoid optimizing away variable
            CPPUNIT_ASSERT(file.getHeaderVersion() ==
                           yapet::HEADER_VERSION::VERSION_2);
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{
                yapet::toSecureArray("JustATestPasswordForKeepingSecret")};
            yapet::BlowfishFactory factory;

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
            auto password{
                yapet::toSecureArray("JustATestPasswordForKeepingSecret")};
            yapet::BlowfishFactory factory;

            YAPET::File file{factory, FN, password, true};
            // avoid optimizing away variable
            CPPUNIT_ASSERT(file.getHeaderVersion() ==
                           yapet::HEADER_VERSION::VERSION_2);
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray("InvalidPassword")};
            yapet::BlowfishFactory factory;

            CPPUNIT_ASSERT_THROW((YAPET::File{factory, FN, password, false}),
                                 YAPET::YAPETInvalidPasswordException);
        } catch (...) {
            CPPUNIT_FAIL("unexpected exception");
        }
    }

    void writePasswords() {
        try {
            auto password{yapet::toSecureArray("Secret")};
            yapet::BlowfishFactory factory;

            auto blowfish{factory.crypto(password)};

            YAPET::File file{factory, FN, password, true};

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

            file.save(passwordList);
        } catch (std::exception &ex) {
            std::cout << std::endl;
            std::cout << typeid(ex).name() << ": " << ex.what() << std::endl;

            CPPUNIT_FAIL("unexpected exception");
        }

        try {
            auto password{yapet::toSecureArray("Secret")};
            yapet::BlowfishFactory factory;

            auto blowfish{factory.crypto(password)};

            YAPET::File file{factory, FN, password, true};

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

    void detectModificationOnSave() { CPPUNIT_FAIL("not implemented"); }

    void forceSave() { CPPUNIT_FAIL("not implemented"); }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(File1Test ::suite());
    return runner.run() ? 0 : 1;
}