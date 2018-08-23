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
#include "yapetfile.hh"

#define TEST_FILE BUILDDIR "/yapet-test-file"

class YapetFileMock : public yapet::YapetFile {
   public:
    YapetFileMock(bool create, bool secure)
        : YapetFile{TEST_FILE, create, secure} {}

    YapetFileMock(YapetFileMock&& other) : YapetFile{std::move(other)} {}

    YapetFile& operator=(YapetFileMock&& other) {
        return yapet::YapetFile::operator=(std::move(other));
    }

    bool hasValidFormat() { throw std::runtime_error("Not implemented"); }

    yapet::SecureArray readIdentifier() {
        throw std::runtime_error("Not implemented");
    }

    yapet::SecureArray readMetaData() {
        throw std::runtime_error("Not implemented");
    }

    std::list<yapet::SecureArray> readPasswordRecords() {
        throw std::runtime_error("Not implemented");
    }

    void writeIdentifier() { throw std::runtime_error("Not implemented"); }

    void writeMetaData(const yapet::SecureArray&) {
        throw std::runtime_error("Not implemented");
    }

    void writePasswordRecords(const std::list<yapet::SecureArray>&) {
        throw std::runtime_error("Not implemented");
    }

    void open() { openRawFile(); }
};

class YapetFileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite* suite() {
        CppUnit::TestSuite* suiteOfTests =
            new CppUnit::TestSuite("YapetFile test");
        suiteOfTests->addTest(new CppUnit::TestCaller<YapetFileTest>{
            "should fail on non existing file",
            &YapetFileTest::throwOnNonExisting});
        suiteOfTests->addTest(new CppUnit::TestCaller<YapetFileTest>{
            "should create new file with secure permissions",
            &YapetFileTest::createNewFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<YapetFileTest>{
            "should refuse to open existing file with insecure permissions",
            &YapetFileTest::failToOpenExistingFileWithInsecurePermissions});
        suiteOfTests->addTest(new CppUnit::TestCaller<YapetFileTest>{
            "should open existing file", &YapetFileTest::openExistingFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<YapetFileTest>{
            "should be forced to open file with insecure permissions",
            &YapetFileTest::forceOpenExistingFileWithInsecurePermissions});

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void throwOnNonExisting() {
        YapetFileMock file{false, false};
        CPPUNIT_ASSERT_THROW(file.open(), yapet::FileError);
    }

    void createNewFile() {
        YapetFileMock file{true, false};
        file.open();

        CPPUNIT_ASSERT(yapet::hasSecurePermissions(TEST_FILE) == true);
    }

    void failToOpenExistingFileWithInsecurePermissions() {
        auto fd = ::open(TEST_FILE, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd == -1) {
            throw std::runtime_error("Failure to create test file");
        }
        ::close(fd);

        YapetFileMock file{false, true};
        CPPUNIT_ASSERT_THROW(file.open(), yapet::FileInsecureError);
    }

    void openExistingFile() {
        auto fd = ::open(TEST_FILE, O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            throw std::runtime_error("Failure to create test file");
        }
        ::close(fd);

        YapetFileMock file{false, true};
        file.open();
        // Not throwing is the test
    }

    void forceOpenExistingFileWithInsecurePermissions() {
        auto fd = ::open(TEST_FILE, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd == -1) {
            throw std::runtime_error("Failure to create test file");
        }
        ::close(fd);

        YapetFileMock file{false, false};
        file.open();
        // Not throwing is the test
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(YapetFileTest::suite());
    return runner.run() ? 0 : 1;
}