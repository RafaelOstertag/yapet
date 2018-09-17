#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "aes256factory.hh"
#include "csvimport.h"
#include "file.h"
#include "testpaths.h"

constexpr auto ROUNDS{10};

constexpr char NAME_FORMAT[]{"Test name %d"};
constexpr char HOST_FORMAT[]{"Test host %d"};
constexpr char USERNAME_FORMAT[]{"Test username %d"};
constexpr char PASSWORD_FORMAT[]{"Test password %d"};
constexpr char COMMENT_FORMAT[]{"Test comment %d"};

class ImportTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite* suite() {
        CppUnit::TestSuite* suiteOfTests =
            new CppUnit::TestSuite("Import Tests");

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "happy path", &ImportTest::happyPath));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "import with errors", &ImportTest::importWithErrors));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "import strange input", &ImportTest::importStrangeInput));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should import with different separator",
            &ImportTest::importWithDifferentSeparator));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should honor double quotes", &ImportTest::importWithDoubleQuotes));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should honor escaped double quotes",
            &ImportTest::importWithEscapedDoubleQuotes));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should honor escaped separators",
            &ImportTest::importWithEscapedSeparators));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should detect erroneous set double quotes",
            &ImportTest::importWithErroneousDoubleQuotes));

        suiteOfTests->addTest(new CppUnit::TestCaller<ImportTest>(
            "should handle double quotes properly",
            &ImportTest::handleDoubleQuotesProperly));

        return suiteOfTests;
    }

    void happyPath() {
        CSVImport imp(SRCDIR "/test1.csv", "test1.pet", ',');
        imp.import("test1");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);
        CPPUNIT_ASSERT(imp.getLog().size() == 0);

        auto password{yapet::toSecureArray("test1")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        auto crypto{cryptoFactory->crypto()};

        std::shared_ptr<YAPET::File> file{
            new YAPET::File{cryptoFactory, "test1.pet", false}};
        std::list<yapet::PasswordListItem> list = file->read();

        CPPUNIT_ASSERT(list.size() == ROUNDS);

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            checkRecord(*it, cryptoFactory, i);
            it++;
        }
    }

    void importWithErrors() {
        CSVImport imp(SRCDIR "/test2.csv", "test2.pet", ',', true);

        imp.import("test2");
        imp.printLog();

        CPPUNIT_ASSERT(imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 2);
        std::list<CSVImport::LogEntry> logs(imp.getLog());

        CPPUNIT_ASSERT(logs.size() == 2);
        std::list<CSVImport::LogEntry>::const_iterator it = logs.begin();

        CPPUNIT_ASSERT((*it).lineNumber == 2);
        it++;
        CPPUNIT_ASSERT((*it).lineNumber == 10);
    }

    void importStrangeInput() {
        CSVImport imp(SRCDIR "/test3.csv", "test3.pet", ',');
        imp.import("test3");

        CPPUNIT_ASSERT(!imp.hadErrors());
    }

    void importWithDifferentSeparator() {
        CSVImport imp(SRCDIR "/test4.csv", "test4.pet", ';');
        imp.import("test4");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);

        auto password{yapet::toSecureArray("test4")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test4.pet", false};
        std::list<yapet::PasswordListItem> list = file.read();

        CPPUNIT_ASSERT(list.size() == ROUNDS);

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            checkRecord(*it, cryptoFactory, i);
            it++;
        }
    }

    void importWithDoubleQuotes() {
        CSVImport imp(SRCDIR "/test5.csv", "test5.pet", ',');
        imp.import("test5");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);

        auto password{yapet::toSecureArray("test5")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test5.pet", false};

        std::list<yapet::PasswordListItem> list = file.read();

        CPPUNIT_ASSERT(list.size() == ROUNDS);

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            checkRecord(*it, cryptoFactory, i);
            it++;
        }
    }

    void importWithEscapedDoubleQuotes() {
        CSVImport imp(SRCDIR "/test6.csv", "test6.pet", ',');
        imp.import("test6");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);

        auto password{yapet::toSecureArray("test6")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test6.pet", false};

        std::list<yapet::PasswordListItem> list = file.read();

        CPPUNIT_ASSERT(list.size() == ROUNDS);

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            checkRecordWithEscapedDoubleQuotes(*it, cryptoFactory, i);
            it++;
        }
    }

    void importWithEscapedSeparators() {
        CSVImport imp(SRCDIR "/test7.csv", "test7.pet", ',');

        imp.import("test7");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);

        auto password{yapet::toSecureArray("test7")};
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory{
            new yapet::Aes256Factory{password}};

        YAPET::File file{cryptoFactory, "test7.pet", false};

        std::list<yapet::PasswordListItem> list = file.read();

        CPPUNIT_ASSERT(list.size() == ROUNDS);

        std::list<yapet::PasswordListItem>::iterator it = list.begin();

        for (int i = 0; it != list.end(); i++) {
            checkRecordWithEscapedSeparator(*it, cryptoFactory, i);
            it++;
        }
    }

    void importWithErroneousDoubleQuotes() {
        CSVImport imp(SRCDIR "/test8.csv", "test8.pet", ',');

        imp.import("test8");
        imp.printLog();

        CPPUNIT_ASSERT(imp.numErrors() == 1);

        std::list<CSVImport::LogEntry> logs(imp.getLog());

        CPPUNIT_ASSERT(logs.size() == 1);

        std::list<CSVImport::LogEntry>::const_iterator it = logs.begin();

        CPPUNIT_ASSERT((*it).lineNumber == 3);
    }

    void handleDoubleQuotesProperly() {
        CSVImport imp(SRCDIR "/test9.csv", "test9.pet", ',');
        imp.import("test9");

        CPPUNIT_ASSERT(!imp.hadErrors());
        CPPUNIT_ASSERT(imp.numErrors() == 0);
    }

    void checkRecord(
        const yapet::PasswordListItem& passwordListItem,
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory,
        int rec_no) {
        auto crypto{cryptoFactory->crypto()};

        char name[yapet::PasswordRecord::NAME_SIZE];
        char host[yapet::PasswordRecord::HOST_SIZE];
        char username[yapet::PasswordRecord::USERNAME_SIZE];
        char password[yapet::PasswordRecord::PASSWORD_SIZE];
        char comment[yapet::PasswordRecord::COMMENT_SIZE];
        std::snprintf(name, yapet::PasswordRecord::NAME_SIZE, NAME_FORMAT,
                      rec_no);
        std::snprintf(host, yapet::PasswordRecord::HOST_SIZE, HOST_FORMAT,
                      rec_no);
        std::snprintf(username, yapet::PasswordRecord::USERNAME_SIZE,
                      USERNAME_FORMAT, rec_no);
        std::snprintf(password, yapet::PasswordRecord::PASSWORD_SIZE,
                      PASSWORD_FORMAT, rec_no);
        std::snprintf(comment, yapet::PasswordRecord::COMMENT_SIZE,
                      COMMENT_FORMAT, rec_no);

        yapet::PasswordRecord passwordRecord{
            crypto->decrypt(passwordListItem.encryptedRecord())};

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordListItem.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordRecord.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(host,
                         reinterpret_cast<const char*>(passwordRecord.host()),
                         yapet::PasswordRecord::HOST_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(username,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.username()),
                                    yapet::PasswordRecord::USERNAME_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(password,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.password()),
                                    yapet::PasswordRecord::PASSWORD_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(comment,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.comment()),
                                    yapet::PasswordRecord::COMMENT_SIZE) == 0);
    }

    void checkRecordWithEscapedSeparator(
        const yapet::PasswordListItem& passwordListItem,
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory,
        int rec_no) {
        auto crypto{cryptoFactory->crypto()};

        char name[yapet::PasswordRecord::NAME_SIZE];
        char host[yapet::PasswordRecord::HOST_SIZE];
        char username[yapet::PasswordRecord::USERNAME_SIZE];
        char password[yapet::PasswordRecord::PASSWORD_SIZE];
        char comment[yapet::PasswordRecord::COMMENT_SIZE];

        std::snprintf(name, yapet::PasswordRecord::NAME_SIZE, ",Test name %d",
                      rec_no);
        std::snprintf(host, yapet::PasswordRecord::HOST_SIZE, "Test, host %d",
                      rec_no);
        std::snprintf(username, yapet::PasswordRecord::USERNAME_SIZE,
                      "Test username, %d", rec_no);
        std::snprintf(password, yapet::PasswordRecord::PASSWORD_SIZE,
                      "Test password %d", rec_no);
        std::snprintf(comment, yapet::PasswordRecord::COMMENT_SIZE,
                      "Test comment %d,", rec_no);

        yapet::PasswordRecord passwordRecord{
            crypto->decrypt(passwordListItem.encryptedRecord())};

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordListItem.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordRecord.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(host,
                         reinterpret_cast<const char*>(passwordRecord.host()),
                         yapet::PasswordRecord::HOST_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(username,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.username()),
                                    yapet::PasswordRecord::USERNAME_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(password,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.password()),
                                    yapet::PasswordRecord::PASSWORD_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(comment,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.comment()),
                                    yapet::PasswordRecord::COMMENT_SIZE) == 0);
    }

    void checkRecordWithEscapedDoubleQuotes(
        const yapet::PasswordListItem& passwordListItem,
        std::shared_ptr<yapet::AbstractCryptoFactory> cryptoFactory,
        int rec_no) {
        auto crypto{cryptoFactory->crypto()};

        char name[yapet::PasswordRecord::NAME_SIZE];
        char host[yapet::PasswordRecord::HOST_SIZE];
        char username[yapet::PasswordRecord::USERNAME_SIZE];
        char password[yapet::PasswordRecord::PASSWORD_SIZE];
        char comment[yapet::PasswordRecord::COMMENT_SIZE];

        std::snprintf(name, yapet::PasswordRecord::NAME_SIZE, "\"Test name %d",
                      rec_no);
        std::snprintf(host, yapet::PasswordRecord::HOST_SIZE, "Test host %d\"",
                      rec_no);
        std::snprintf(username, yapet::PasswordRecord::USERNAME_SIZE,
                      "Test \"username %d", rec_no);
        std::snprintf(password, yapet::PasswordRecord::PASSWORD_SIZE,
                      "Test \"password\" %d", rec_no);
        std::snprintf(comment, yapet::PasswordRecord::COMMENT_SIZE,
                      "Test \"\"comment %d\"\"", rec_no);

        yapet::PasswordRecord passwordRecord{
            crypto->decrypt(passwordListItem.encryptedRecord())};

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordListItem.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(name,
                         reinterpret_cast<const char*>(passwordRecord.name()),
                         yapet::PasswordRecord::NAME_SIZE) == 0);

        CPPUNIT_ASSERT(
            std::strncmp(host,
                         reinterpret_cast<const char*>(passwordRecord.host()),
                         yapet::PasswordRecord::HOST_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(username,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.username()),
                                    yapet::PasswordRecord::USERNAME_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(password,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.password()),
                                    yapet::PasswordRecord::PASSWORD_SIZE) == 0);

        CPPUNIT_ASSERT(std::strncmp(comment,
                                    reinterpret_cast<const char*>(
                                        passwordRecord.comment()),
                                    yapet::PasswordRecord::COMMENT_SIZE) == 0);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(ImportTest::suite());
    return runner.run() ? 0 : 1;
}
