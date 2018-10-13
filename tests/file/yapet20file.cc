#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fileerror.hh"
#include "fileutils.hh"
#include "testpaths.h"
#include "yapet20file.hh"

constexpr auto TEST_FILE{BUILDDIR "/yapet-yapet20file-test"};

class Yapet20FileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite* suite() {
        CppUnit::TestSuite* suiteOfTests =
            new CppUnit::TestSuite("Yapet 2.0 File test");
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should not fail opening a new file",
            &Yapet20FileTest::openNewFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should fail reading empty identifier",
            &Yapet20FileTest::failOpeningEmptyFile});

        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should fail opening a file with wrong identifier",
            &Yapet20FileTest::failOpeningFileWithWrongIdentifier});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should read identifier", &Yapet20FileTest::readIdentifier});

        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should fail reading empty meta data",
            &Yapet20FileTest::failReadingEmptyMetaData});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should read meta data", &Yapet20FileTest::readMetaData});

        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should fail reading empty header",
            &Yapet20FileTest::failReadingEmptyHeader});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should read header", &Yapet20FileTest::readHeader});

        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should return empty password list",
            &Yapet20FileTest::readNoPasswords});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should read password records",
            &Yapet20FileTest::readPasswordRecords});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should move", &Yapet20FileTest::moveConstructor});
        suiteOfTests->addTest(new CppUnit::TestCaller<Yapet20FileTest>{
            "should move assign", &Yapet20FileTest::moveAssignment});

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void openNewFile() {
        yapet::Yapet20File newFile{TEST_FILE, true, false};
        newFile.open();
    }

    void failOpeningEmptyFile() {
        int fd = ::open(TEST_FILE, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
        CPPUNIT_ASSERT(fd > -1);
        ::close(fd);

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        CPPUNIT_ASSERT_THROW(yapet20File.open(), yapet::FileFormatError);
    }

    void failOpeningFileWithWrongIdentifier() {
        yapet::Yapet20File yapet20File{
            BUILDDIR "/yapet20file-corrupt-identifier.pet", false, false};
        CPPUNIT_ASSERT_THROW(yapet20File.open(), yapet::FileFormatError);
    }

    void readIdentifier() {
        // try/catch block to close file when yapet20File goes out of scope
        try {
            yapet::Yapet20File yapet20File{TEST_FILE, true, false};
            yapet20File.open();
            yapet20File.writeIdentifier();
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet20File yapet20File2{TEST_FILE, false, false};
        yapet20File2.open();
        CPPUNIT_ASSERT(
            std::memcmp(*yapet20File2.readIdentifier(), "YAPET2.0", 8) == 0);
    }

    void failReadingEmptyMetaData() {
        yapet::Yapet20File yapet20File{TEST_FILE, true, false};
        yapet20File.open();
        yapet20File.writeIdentifier();

        CPPUNIT_ASSERT_THROW(yapet20File.readHeader(), yapet::FileFormatError);
    }

    void readMetaData() {
        yapet::SecureArray metaData{yapet::toSecureArray("metadata")};
        try {
            yapet::Yapet20File yapet20File{TEST_FILE, true, false};
            yapet20File.open();
            yapet20File.writeIdentifier();

            yapet20File.writeUnencryptedMetaData(metaData);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();
        auto actual = yapet20File.readUnencryptedMetaData();

        CPPUNIT_ASSERT(metaData == actual);
    }

    void failReadingEmptyHeader() {
        yapet::Yapet20File yapet20File{TEST_FILE, true, false};
        yapet20File.open();
        yapet20File.writeIdentifier();
        CPPUNIT_ASSERT_THROW(yapet20File.readHeader(), yapet::FileFormatError);

        yapet::SecureArray metaData{yapet::toSecureArray("metadata")};
        yapet20File.writeUnencryptedMetaData(metaData);
        CPPUNIT_ASSERT_THROW(yapet20File.readHeader(), yapet::FileFormatError);
    }

    void readHeader() {
        yapet::SecureArray headerData(2);
        (*headerData)[0] = 'A';
        (*headerData)[1] = 'B';
        // try/catch block to close file when yapet20File goes out of scope
        try {
            yapet::Yapet20File yapet20File{TEST_FILE, true, false};
            yapet20File.open();
            yapet20File.writeIdentifier();
            yapet::SecureArray metaData{yapet::toSecureArray("metadata")};
            yapet20File.writeUnencryptedMetaData(metaData);

            yapet20File.writeHeader(headerData);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();
        auto actual = yapet20File.readHeader();

        CPPUNIT_ASSERT(actual == headerData);
    }

    void readNoPasswords() {
        // try/catch block to close file when yapet20File goes out of scope
        try {
            yapet::Yapet20File yapet20File{TEST_FILE, true, false};
            yapet20File.open();
            yapet20File.writeIdentifier();
            yapet::SecureArray metaData{yapet::toSecureArray("metadata")};
            yapet20File.writeUnencryptedMetaData(metaData);

            yapet::SecureArray headerData(1);
            (*headerData)[0] = 'M';
            yapet20File.writeHeader(headerData);
        } catch (...) {
            CPPUNIT_FAIL("No exception expected");
        }

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();

        auto passwords = yapet20File.readPasswordRecords();
        CPPUNIT_ASSERT(passwords.size() == 0);
    }

    void writeTestPasswords() {
        yapet::Yapet20File yapet20File{TEST_FILE, true, false};
        yapet20File.open();
        yapet20File.writeIdentifier();
        yapet::SecureArray metaData{yapet::toSecureArray("metadata")};
        yapet20File.writeUnencryptedMetaData(metaData);

        yapet::SecureArray headerData(1);
        (*headerData)[0] = 'M';
        yapet20File.writeHeader(headerData);

        std::list<yapet::SecureArray> passwords;
        for (auto i = 0; i < 5; i++) {
            yapet::SecureArray password(1);
            **password = i;
            passwords.push_back(password);
        }

        yapet20File.writePasswordRecords(passwords);
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

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();
        auto passwords = yapet20File.readPasswordRecords();

        testPasswordRecords(passwords);
    }

    void moveConstructor() {
        writeTestPasswords();

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();

        yapet::Yapet20File yapet10FileMoved{std::move(yapet20File)};

        CPPUNIT_ASSERT(yapet10FileMoved.isSecure() == false);
        CPPUNIT_ASSERT(yapet10FileMoved.isCreate() == false);

        auto passwords = yapet10FileMoved.readPasswordRecords();
        testPasswordRecords(passwords);
    }

    void moveAssignment() {
        writeTestPasswords();

        yapet::Yapet20File yapet20File{TEST_FILE, false, false};
        yapet20File.open();

        yapet::Yapet20File yapet10FileMoved = std::move(yapet20File);

        CPPUNIT_ASSERT(yapet10FileMoved.isSecure() == false);
        CPPUNIT_ASSERT(yapet10FileMoved.isCreate() == false);

        auto passwords = yapet10FileMoved.readPasswordRecords();
        testPasswordRecords(passwords);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Yapet20FileTest::suite());
    return runner.run() ? 0 : 1;
}