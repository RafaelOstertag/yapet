// Testing whether or not files generated on the author's machine can be read
// on other machines as well...

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
#include "file.hh"
#include "securearray.hh"
#include "testpaths.h"
#include "openssl.hh"

constexpr auto TEST_PASSWORD{"test1"};

constexpr auto ROUNDS{200};

constexpr auto NAME{"Test name"};
constexpr auto HOST{"Test host"};
constexpr auto USERNAME{"Test username"};
constexpr auto PASSWORD{"Test password"};
constexpr auto COMMENT{"Test comment"};

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

inline void testFile(const char *filename) {
    auto password{yapet::toSecureArray(TEST_PASSWORD)};
    std::shared_ptr<yapet::AbstractCryptoFactory> factory{
        new yapet::BlowfishFactory{password, yapet::MetaData{}}};

    auto crypto{factory->crypto()};

    YAPET::File file{factory, filename, false, false};

    auto passwords{file.read()};
    CPPUNIT_ASSERT(passwords.size() == ROUNDS);

    std::list<yapet::PasswordListItem>::iterator it = passwords.begin();
    for (int i = 0; it != passwords.end(); i++, it++) {
        auto name{makeName(i)};
        CPPUNIT_ASSERT(std::strcmp(name.c_str(), reinterpret_cast<const char *>(
                                                     it->name())) == 0);

        auto serializedPasswordRecord{crypto->decrypt(it->encryptedRecord())};
        yapet::PasswordRecord actual{serializedPasswordRecord};

        yapet::PasswordRecord expected{makePasswordRecord(i)};
        comparePasswordRecords(actual, expected);
    }
}

class ForeignTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Blowfish");

        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "32bit little endian pre 0.6", &ForeignTest::test32lePre06));
        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "32bit big endian pre 0.6", &ForeignTest::test32bePre06));

        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "64bit little endian pre 0.6", &ForeignTest::test64lePre06));
        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "64bit big endian pre 0.6", &ForeignTest::test64bePre06));

        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "32bit little endian 0.6", &ForeignTest::test32le06));
        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "32bit big endian 0.6", &ForeignTest::test32be06));

        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "64bit little endian 0.6", &ForeignTest::test64le06));
        suiteOfTests->addTest(new CppUnit::TestCaller<ForeignTest>(
            "64bit big endian 0.6", &ForeignTest::test64be06));

        return suiteOfTests;
    }

    void test32lePre06() { testFile(BUILDDIR "/f32le0.5.pet"); }

    void test32bePre06() { testFile(BUILDDIR "/f32be0.5.pet"); }

    void test64lePre06() { testFile(BUILDDIR "/f64le0.5.pet"); }

    void test64bePre06() { testFile(BUILDDIR "/f64be0.5.pet"); }

    void test32le06() { testFile(BUILDDIR "/f32le0.6.pet"); }

    void test32be06() { testFile(BUILDDIR "/f32be0.6.pet"); }

    void test64le06() { testFile(BUILDDIR "/f64le0.6.pet"); }

    void test64be06() { testFile(BUILDDIR "/f64be0.6.pet"); }
};

int main() {
    yapet::OpenSSL::init();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(ForeignTest ::suite());
    return runner.run() ? 0 : 1;
}
