#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <unistd.h>
#include "file.hh"
#include "testpaths.h"

#define TEST_FILE BUILDDIR "/yapet-test-file"

class FileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("File test");
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should throw on open already open file",
            &FileTest::throwOnOpenAlreadyOpen});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should throw on non-open file read/write",
            &FileTest::throwOnNonOpenReadWrite});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should throw open non-existing file",
            &FileTest::throwOnOpenNonExistingFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should read/write", &FileTest::testReadWrite});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should read/write secure array",
            &FileTest::testReadWriteSecureArray});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileTest>{
            "should read past EOF without error", &FileTest::testReadPastEof});

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void throwOnOpenAlreadyOpen() {
        yapet::File file{TEST_FILE};
        file.openNew();
        CPPUNIT_ASSERT_THROW(file.openExisting(), yapet::FileError);

        yapet::File file2{TEST_FILE};
        file2.openExisting();
        CPPUNIT_ASSERT_THROW(file2.openNew(), yapet::FileError);
    }

    void throwOnNonOpenReadWrite() {
        yapet::File file{TEST_FILE};

        CPPUNIT_ASSERT_THROW(file.rewind(), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.read(), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.read(1), yapet::FileError);
        yapet::SecureArray array{1};

        CPPUNIT_ASSERT_THROW(file.write(array), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.write(*array, 1), yapet::FileError);
    }

    void throwOnOpenNonExistingFile() {
        yapet::File file{BUILDDIR "/must-not-exist"};

        CPPUNIT_ASSERT_THROW(file.openExisting(), yapet::FileError);
    }

    void testReadWrite() {
        yapet::File file{TEST_FILE};

        file.openNew();

        std::uint8_t fileContent[]{'t', 'e', 's', 't'};
        auto fileContentSize = sizeof(fileContent);
        file.write(fileContent, fileContentSize);

        file.rewind();

        auto actualPair = file.read(sizeof(fileContent));
        CPPUNIT_ASSERT_EQUAL(true, actualPair.second);
        auto actual = actualPair.first;

        CPPUNIT_ASSERT_EQUAL((yapet::SecureArray::size_type)fileContentSize,
                             actual.size());

        for (auto i = 0; i < (int)fileContentSize; i++) {
            CPPUNIT_ASSERT_EQUAL(fileContent[i], actual[i]);
        }
    }

    void testReadPastEof() {
        yapet::File file{TEST_FILE};

        file.openNew();

        auto actualPair = file.read(1);
        CPPUNIT_ASSERT_EQUAL(false, actualPair.second);

        actualPair = file.read();
        CPPUNIT_ASSERT_EQUAL(false, actualPair.second);
    }

    void testReadWriteSecureArray() {
        yapet::SecureArray secureArray(5);
        for (auto i = 0; i < 5; i++) {
            (*secureArray)[i] = i;
        }

        yapet::File file{TEST_FILE};
        file.openNew();
        file.write(secureArray);
        file.rewind();

        auto actualPair = file.read();
        CPPUNIT_ASSERT_EQUAL(true, actualPair.second);

        auto actual = actualPair.first;
        CPPUNIT_ASSERT_EQUAL((yapet::SecureArray::size_type)5, actual.size());

        for (auto i = 0; i < 5; i++) {
            CPPUNIT_ASSERT_EQUAL((std::uint8_t)i, actual[i]);
        }
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(FileTest::suite());
    runner.run();
    return 0;
}