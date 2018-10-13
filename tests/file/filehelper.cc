#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <typeinfo>

#include "fileerror.hh"
#include "filehelper.hh"
#include "testpaths.h"
#include "yapet10file.hh"
#include "yapet20file.hh"

constexpr auto TEST_FILE{BUILDDIR "/yapet-filehelper-test"};

class FileHelperTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("File Helper Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "reading file identifier should fail on empty file",
            &FileHelperTest::readEmptyFile});

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "reading file identifier should fail on unknown file",
            &FileHelperTest::failReadIdentifierUnknownFile});

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should read YAPET 1.0 file identifier",
            &FileHelperTest::readYapet10FileIdentifier});

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should read YAPET 2.0 file identifier",
            &FileHelperTest::readYapet20FileIdentifier});

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should get YAPET 1.0 file", &FileHelperTest::getYapet10File});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should get YAPET 2.0 file", &FileHelperTest::getYapet20File});

        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should read YAPET 1.0 meta data",
            &FileHelperTest::readYapet10MetaData});
        suiteOfTests->addTest(new CppUnit::TestCaller<FileHelperTest>{
            "should read YAPET 2.0 meta data",
            &FileHelperTest::readYapet20MetaData});

        return suiteOfTests;
    }

    int openFile() {
        auto fd = ::open(TEST_FILE, O_CREAT | O_TRUNC | O_RDWR,
                         S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd < 0) {
            throw std::runtime_error{"Error creating file"};
        }

        return fd;
    }

    void createEmptyFile() {
        auto fd = openFile();

        ::close(fd);
    }

    void setUp() { ::unlink(TEST_FILE); }

    void tearDown() { ::unlink(TEST_FILE); }

    void readEmptyFile() {
        createEmptyFile();

        CPPUNIT_ASSERT_THROW(yapet::readFileIdentifier(TEST_FILE),
                             yapet::FileFormatError);
    }

    void failReadIdentifierUnknownFile() {
        int fd{openFile()};

        write(fd, "bla", 3);
        close(fd);

        CPPUNIT_ASSERT_THROW(yapet::readFileIdentifier(TEST_FILE),
                             yapet::FileFormatError);
    }

    void makeYapet10File() {
        int fd{openFile()};

        write(fd, yapet::Yapet10File::RECOGNITION_STRING,
              yapet::Yapet10File::RECOGNITION_STRING_SIZE);
        close(fd);
    }

    void readYapet10FileIdentifier() {
        makeYapet10File();

        auto actual{yapet::readFileIdentifier(TEST_FILE)};
        auto expected{
            yapet::toSecureArray(yapet::Yapet10File::RECOGNITION_STRING,
                                 yapet::Yapet10File::RECOGNITION_STRING_SIZE)};

        CPPUNIT_ASSERT(actual == expected);

        CPPUNIT_ASSERT(yapet::isFileType(expected, TEST_FILE));

        auto unexpected{
            yapet::toSecureArray(yapet::Yapet20File::RECOGNITION_STRING,
                                 yapet::Yapet20File::RECOGNITION_STRING_SIZE)};
        CPPUNIT_ASSERT(!yapet::isFileType(unexpected, TEST_FILE));
    }

    void makeYapet20File() {
        int fd{openFile()};

        write(fd, yapet::Yapet20File::RECOGNITION_STRING,
              yapet::Yapet20File::RECOGNITION_STRING_SIZE);
        close(fd);
    }

    void readYapet20FileIdentifier() {
        makeYapet20File();

        auto actual{yapet::readFileIdentifier(TEST_FILE)};
        auto expected{
            yapet::toSecureArray(yapet::Yapet20File::RECOGNITION_STRING,
                                 yapet::Yapet20File::RECOGNITION_STRING_SIZE)};

        CPPUNIT_ASSERT(actual == expected);

        CPPUNIT_ASSERT(yapet::isFileType(expected, TEST_FILE));

        auto unexpected{
            yapet::toSecureArray(yapet::Yapet10File::RECOGNITION_STRING,
                                 yapet::Yapet10File::RECOGNITION_STRING_SIZE)};

        CPPUNIT_ASSERT(!yapet::isFileType(unexpected, TEST_FILE));
    }

    void getYapet10File() {
        yapet::SecureArray metaData{};
        yapet::SecureArray header{yapet::toSecureArray("Header")};
        try {
            yapet::Yapet10File file{TEST_FILE, true, false};
            file.open();
            file.writeIdentifier();
            file.writeUnencryptedMetaData(metaData);
            file.writeHeader(header);
        } catch (...) {
            CPPUNIT_FAIL("Unexpected exception");
        }

        auto file{yapet::getFile(TEST_FILE, false)};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*file) == typeid(yapet::Yapet10File));
#pragma clang diagnostic pop
    }

    void getYapet20File() {
        yapet::SecureArray metaData{yapet::toSecureArray("MetaData")};
        yapet::SecureArray header{yapet::toSecureArray("Header")};
        try {
            yapet::Yapet20File file{TEST_FILE, true, false};
            file.open();
            file.writeIdentifier();
            file.writeUnencryptedMetaData(metaData);
            file.writeHeader(header);
        } catch (...) {
            CPPUNIT_FAIL("Unexpected exception");
        }

        auto file{yapet::getFile(TEST_FILE, false)};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*file) == typeid(yapet::Yapet20File));
#pragma clang diagnostic pop
    }

    void readYapet10MetaData() {
        yapet::MetaData metaData{};
        yapet::SecureArray header{yapet::toSecureArray("Header")};
        try {
            yapet::Yapet10File file{TEST_FILE, true, false};
            file.open();
            file.writeIdentifier();
            file.writeUnencryptedMetaData(metaData.serialize());
            file.writeHeader(header);
        } catch (...) {
            CPPUNIT_FAIL("Unexpected exception");
        }

        auto actual{yapet::readMetaData(TEST_FILE, false)};
        CPPUNIT_ASSERT(actual.size() == metaData.size());
    }

    void readYapet20MetaData() {
        yapet::MetaData metaData{};
        metaData.setValue("a", 42);
        metaData.setValue("b", 84);

        yapet::SecureArray header{yapet::toSecureArray("Header")};
        try {
            yapet::Yapet20File file{TEST_FILE, true, false};
            file.open();
            file.writeIdentifier();
            file.writeUnencryptedMetaData(metaData.serialize());
            file.writeHeader(header);
        } catch (...) {
            CPPUNIT_FAIL("Unexpected exception");
        }

        auto actual{yapet::readMetaData(TEST_FILE, false)};
        CPPUNIT_ASSERT(actual.size() == metaData.size());
        CPPUNIT_ASSERT(actual.getValue("a") == metaData.getValue("a"));
        CPPUNIT_ASSERT(actual.getValue("b") == metaData.getValue("b"));
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(FileHelperTest::suite());
    return runner.run() ? 0 : 1;
}