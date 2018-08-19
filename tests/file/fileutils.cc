#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "fileutils.hh"
#include "fileerror.hh"
#include "testpaths.h"

#define TEST_FILE BUILDDIR "/yapet-test-file"

class FileUtilsTest : public CppUnit::TestFixture {
public:

    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
                new CppUnit::TestSuite("File utils test");
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "should set secure permissions and owner",
            &FileUtilsTest::setSecurePermissionsAndOwner
        });
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "setting secure permissions and owner should throw",
            &FileUtilsTest::setSecurePermissionsAndOwnerNonExisting
        });
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "testing secure permissions and owner should throw",
            &FileUtilsTest::hasSecurePermissionsAndOwnerNonExisting
        });
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "testing secure permissions and owner should throw",
            &FileUtilsTest::getModificationTime
        });
        suiteOfTests->addTest(new CppUnit::TestCaller<FileUtilsTest>{
            "testing secure permissions and owner should throw",
            &FileUtilsTest::getModificationTimeNonExisting
        });


        return suiteOfTests;
    }

    void setUp() {
        ::unlink(TEST_FILE);
    }

    void tearDown() {
        ::unlink(TEST_FILE);
    }

    void createInsecureFile() {
        auto fd = ::open(TEST_FILE, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd < 0) {
            throw std::runtime_error{"Error creating file"};
        }

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
        CPPUNIT_ASSERT_THROW(yapet::setSecurePermissionsAndOwner("must-not-exist"), yapet::FileError);

    }

    void hasSecurePermissionsAndOwnerNonExisting() {
        CPPUNIT_ASSERT_THROW(yapet::hasSecurePermissions("must-not-exist"), yapet::FileError);
    }

    void getModificationTime() {
        auto currentTime = ::time(nullptr);
        createFile();

        auto modificationTime = yapet::getModificationTime(TEST_FILE);
        CPPUNIT_ASSERT(currentTime <= modificationTime);
    }

    void getModificationTimeNonExisting() {
        CPPUNIT_ASSERT_THROW(yapet::getModificationTime("must-not-exist"), yapet::FileError);
    }

};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(FileUtilsTest::suite());
    return runner.run() ? 0 : 1;
}