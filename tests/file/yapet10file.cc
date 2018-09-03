#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fileerror.hh"
#include "fileutils.hh"
#include "testpaths.h"
#include "yapet10file.hh"

constexpr auto TEST_FILE {BUILDDIR "/yapet-yapet10file-test"};

class Yapet10FileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite* suite() {
        CppUnit::TestSuite* suiteOfTests =
            new CppUnit::TestSuite("Yapet10File test");
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should fail reading empty identifier",
            &Yapet10FileTest::failReadingEmptyIdentifier});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should read identifier", &Yapet10FileTest::readIdentifier});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should fail reading empty meta data",
            &Yapet10FileTest::failReadingEmptyMetaData});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should read meta data", &Yapet10FileTest::readMetaData});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should return empty password list",
            &Yapet10FileTest::readNoPasswords});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should read password records",
            &Yapet10FileTest::readPasswordRecords});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should move", &Yapet10FileTest::moveConstructor});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet10FileTest>{
            "should move assign", &Yapet10FileTest::moveAssignment});

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void failReadingEmptyIdentifier() {
        yapet::Yapet10File yapet10File{TEST_FILE, true, false};

        CPPUNIT_ASSERT_THROW(yapet10File.readIdentifier(),
                             yapet::FileFormatError);

        yapet::Yapet10File yapet10File2{TEST_FILE, true, false};

        CPPUNIT_ASSERT(yapet10File2.hasValidFormat() == false);
    }

    void readIdentifier() {
        // try/catch block to close file when yapet10File goes out of scope
        try {
            yapet::Yapet10File yapet10File{TEST_FILE, true, false};
            yapet10File.writeIdentifier();

            CPPUNIT_ASSERT(yapet10File.hasValidFormat() == true);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet10File yapet10File2{TEST_FILE, false, false};
        CPPUNIT_ASSERT(yapet10File2.hasValidFormat() == true);
    }

    void failReadingEmptyMetaData() {
        yapet::Yapet10File yapet10File{TEST_FILE, true, false};
        yapet10File.writeIdentifier();

        CPPUNIT_ASSERT_THROW(yapet10File.readMetaData(),
                             yapet::FileFormatError);
    }

    void readMetaData() {
        // try/catch block to close file when yapet10File goes out of scope
        try {
            yapet::Yapet10File yapet10File{TEST_FILE, true, false};
            yapet10File.writeIdentifier();

            yapet::SecureArray metaData(2);
            (*metaData)[0] = 'A';
            (*metaData)[1] = 'B';
            yapet10File.writeMetaData(metaData);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet10File yapet10File{TEST_FILE, false, false};
        auto metaData = yapet10File.readMetaData();

        CPPUNIT_ASSERT((*metaData)[0] == 'A' && (*metaData)[1] == 'B');
    }

    void readNoPasswords() {
        // try/catch block to close file when yapet10File goes out of scope
        try {
            yapet::Yapet10File yapet10File{TEST_FILE, true, false};
            yapet10File.writeIdentifier();

            yapet::SecureArray metaData(1);
            (*metaData)[0] = 'M';
            yapet10File.writeMetaData(metaData);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet10File yapet10File{TEST_FILE, false, false};
        auto passwords = yapet10File.readPasswordRecords();
        CPPUNIT_ASSERT(passwords.size() == 0);
    }

    void writeTestPasswords() {
        yapet::Yapet10File yapet10File{TEST_FILE, true, false};
        yapet10File.writeIdentifier();

        yapet::SecureArray metaData(1);
        (*metaData)[0] = 'M';
        yapet10File.writeMetaData(metaData);

        std::list<yapet::SecureArray> passwords;
        for (auto i = 0; i < 5; i++) {
            yapet::SecureArray password(1);
            **password = i;
            passwords.push_back(password);
        }

        yapet10File.writePasswordRecords(passwords);
    }

    void testPasswordRecords(std::list<yapet::SecureArray> passwords) {
        CPPUNIT_ASSERT(passwords.size() == 5);

        auto expectedValue{0};
        for (auto password : passwords) {
            CPPUNIT_ASSERT(password.size() == 1);
            CPPUNIT_ASSERT(**password == expectedValue++);
        }
    }

    void readPasswordRecords() {
        writeTestPasswords();

        yapet::Yapet10File yapet10File{TEST_FILE, false, false};
        auto passwords = yapet10File.readPasswordRecords();

        testPasswordRecords(passwords);
    }

    void moveConstructor() {
        writeTestPasswords();

        yapet::Yapet10File yapet10File{TEST_FILE, false, false};

        yapet::Yapet10File yapet10FileMoved{std::move(yapet10File)};

        CPPUNIT_ASSERT(yapet10FileMoved.isSecure() == false);
        CPPUNIT_ASSERT(yapet10FileMoved.isCreate() == false);

        auto passwords = yapet10FileMoved.readPasswordRecords();
        testPasswordRecords(passwords);
    }

    void moveAssignment() {
        writeTestPasswords();

        yapet::Yapet10File yapet10File{TEST_FILE, false, false};

        yapet::Yapet10File yapet10FileMoved = std::move(yapet10File);

        CPPUNIT_ASSERT(yapet10FileMoved.isSecure() == false);
        CPPUNIT_ASSERT(yapet10FileMoved.isCreate() == false);

        auto passwords = yapet10FileMoved.readPasswordRecords();
        testPasswordRecords(passwords);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Yapet10FileTest::suite());
    return runner.run() ? 0 : 1;
}