#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <unistd.h>
#include "fileerror.hh"
#include "rawfile.hh"
#include "testpaths.h"

#define TEST_FILE BUILDDIR "/yapet-test-file"

class RawFileTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("RawFile test");
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should throw on open already open file",
            &RawFileTest::throwOnOpenAlreadyOpen});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should throw on non-open file read/write",
            &RawFileTest::throwOnNonOpenReadWrite});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should throw open non-existing file",
            &RawFileTest::throwOnOpenNonExistingFile});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should read/write", &RawFileTest::testReadWrite});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should read/write secure array",
            &RawFileTest::testReadWriteSecureArray});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should read past EOF without error",
            &RawFileTest::testReadPastEof});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should not seek negative",
            &RawFileTest::testSeekAbsoluteNegative});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should seek absolute", &RawFileTest::testSeekAbsolute});
        suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should close file", &RawFileTest::testClose});
            suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should re-open file", &RawFileTest::testReopen});
            suiteOfTests->addTest(new CppUnit::TestCaller<RawFileTest>{
            "should get current position", &RawFileTest::testGetCurrentPosition});
        

        return suiteOfTests;
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void throwOnOpenAlreadyOpen() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();
        CPPUNIT_ASSERT_THROW(file.openExisting(), yapet::FileError);

        yapet::RawFile file2{TEST_FILE};
        file2.openExisting();
        CPPUNIT_ASSERT_THROW(file2.openNew(), yapet::FileError);
    }

    void throwOnNonOpenReadWrite() {
        yapet::RawFile file{TEST_FILE};

        CPPUNIT_ASSERT_THROW(file.rewind(), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.read(), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.read(1), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.seekAbsolute(1), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.reopen(), yapet::FileError);

        yapet::SecureArray array{1};
        CPPUNIT_ASSERT_THROW(file.write(array), yapet::FileError);
        CPPUNIT_ASSERT_THROW(file.write(*array, 1), yapet::FileError);
    }

    void throwOnOpenNonExistingFile() {
        yapet::RawFile file{BUILDDIR "/must-not-exist"};

        CPPUNIT_ASSERT_THROW(file.openExisting(), yapet::FileError);
    }

    void testReadWrite() {
        yapet::RawFile file{TEST_FILE};

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
        yapet::RawFile file{TEST_FILE};

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

        yapet::RawFile file{TEST_FILE};
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

    void testSeekAbsoluteNegative() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();

        CPPUNIT_ASSERT_THROW(file.seekAbsolute(-1), std::invalid_argument);
    }

    void testSeekAbsolute() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();

        file.seekAbsolute(10);

        yapet::SecureArray secureArray{1};
        **secureArray = 'A';

        file.write(secureArray);

        file.seekAbsolute(10);
        auto actualPair = file.read();
        CPPUNIT_ASSERT_EQUAL(true, actualPair.second);

        CPPUNIT_ASSERT_EQUAL((std::uint8_t)'A', **(actualPair.first));
    }

    void testClose() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();

        CPPUNIT_ASSERT(file.isOpen() == true);

        file.close();
        CPPUNIT_ASSERT(file.isOpen() == false);
    }

    void testReopen() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();

        yapet::SecureArray secureArray{1};
        **secureArray = 'A';

        file.write(secureArray);
        file.reopen();

        auto resultPair = file.read();
        CPPUNIT_ASSERT(resultPair.second == true);
        CPPUNIT_ASSERT(**resultPair.first == 'A');

        file.write(secureArray);
            }

    void testGetCurrentPosition() {
        yapet::RawFile file{TEST_FILE};
        file.openNew();

        CPPUNIT_ASSERT(file.getPosition() == 0);

        yapet::SecureArray secureArray{1};
        **secureArray = 'A';

        file.write(secureArray);
        CPPUNIT_ASSERT(file.getPosition() == (sizeof(yapet::SecureArray::size_type)+1));
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RawFileTest::suite());
    return runner.run() ? 0 : 1;
}