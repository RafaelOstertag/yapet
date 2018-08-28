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

#define TEST_FILE BUILDDIR "/yapet-test-file"

class FileUtilsTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("File utils test");
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should set secure permissions and owner",
            &FileUtilsTest::setSecurePermissionsAndOwner});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "setting secure permissions and owner should throw",
            &FileUtilsTest::setSecurePermissionsAndOwnerNonExisting});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "testing secure permissions and owner should throw",
            &FileUtilsTest::hasSecurePermissionsAndOwnerNonExisting});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should get modification time",
            &FileUtilsTest::getModificationTime});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should throw on getting modification time of non-existing file",
            &FileUtilsTest::getModificationTimeNonExisting});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should throw on renaming non existing file",
            &FileUtilsTest::renameNonExistingFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should rename file", &FileUtilsTest::renameFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should throw on renaming to same name",
            &FileUtilsTest::renameFileToSameName});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should get filesize", &FileUtilsTest::getFileSize});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "getFileSize should throw on non-existing file",
            &FileUtilsTest::getFileSizeNonExisting});

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    int openInsecureFile() {
        auto fd = ::open(TEST_FILE, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd < 0) {
            throw std::runtime_error{"Error creating file"};
        }

        return fd;
    }

    void createInsecureFile() {
        auto fd = openInsecureFile();

        ::close(fd);
    }

    void createFile() {
        // currently only used for the modification test, which does not care
        // about permissions
        createInsecureFile();
    }

    void setSecurePermissionsAndOwner() {
        createInsecureFile();

        CPPUNIT_ASSERT_EQUAL(false, yapet::hasSecurePermissions(TEST_FILE));

        yapet::setSecurePermissionsAndOwner(TEST_FILE);

        CPPUNIT_ASSERT_EQUAL(true, yapet::hasSecurePermissions(TEST_FILE));
    }

    void setSecurePermissionsAndOwnerNonExisting() {
        CPPUNIT_ASSERT_THROW(
            yapet::setSecurePermissionsAndOwner("must-not-exist"),
            yapet::FileError);
    }

    void hasSecurePermissionsAndOwnerNonExisting() {
        CPPUNIT_ASSERT_THROW(yapet::hasSecurePermissions("must-not-exist"),
                             yapet::FileError);
    }

    void getModificationTime() {
        auto currentTime = ::time(nullptr);
        createFile();

        auto modificationTime = yapet::getModificationTime(TEST_FILE);
        CPPUNIT_ASSERT(currentTime <= modificationTime);
    }

    void getModificationTimeNonExisting() {
        CPPUNIT_ASSERT_THROW(yapet::getModificationTime("must-not-exist"),
                             yapet::FileError);
    }

    void renameNonExistingFile() {
        CPPUNIT_ASSERT_THROW(yapet::renameFile("must-not-exist", "wdc"),
                             yapet::FileError);
    }

    void renameFile() {
        createFile();

        yapet::renameFile(TEST_FILE, TEST_FILE ".renamed");

        struct stat wdc;
        CPPUNIT_ASSERT_EQUAL(0, ::stat(TEST_FILE ".renamed", &wdc));

        // Rename it back so it will be deleted by tearDown()
        yapet::renameFile(TEST_FILE ".renamed", TEST_FILE);

        CPPUNIT_ASSERT_EQUAL(0, ::stat(TEST_FILE, &wdc));
    }

    void renameFileToSameName() {
        createFile();

        CPPUNIT_ASSERT_THROW(yapet::renameFile(TEST_FILE, TEST_FILE),
                             yapet::FileError);
    }

    void getFileSize() {
        createFile();

        CPPUNIT_ASSERT(yapet::getFileSize(TEST_FILE) == 0);

        auto fd = openInsecureFile();
        auto buff = "test";
        ::write(fd, &buff, 4);
        ::close(fd);

        CPPUNIT_ASSERT(yapet::getFileSize(TEST_FILE) == 0);
    }

    void getFileSizeNonExisting() {
        CPPUNIT_ASSERT_THROW(yapet::getFileSize("must-not-exist"),
                             yapet::FileError);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(FileUtilsTest::suite());
    return runner.run() ? 0 : 1;
}