#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "csvstringfield.hh"

class CSVStringFieldTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("CSV String Field");

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should escape field", &CSVStringFieldTest::escape));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly handle escapeing double quote",
            &CSVStringFieldTest::escapeDoubleQuote));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should not escape field", &CSVStringFieldTest::noEscape));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "escape should properly handle empty string",
            &CSVStringFieldTest::escapeEmptyField));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should unescape escaped field", &CSVStringFieldTest::unescape));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should not unescape wrongly escaped field",
            &CSVStringFieldTest::unescapeWronglyEscapedField));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should not unescape non-escaped field",
            &CSVStringFieldTest::noUnescape));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly handle empty field",
            &CSVStringFieldTest::unescapeEmptyField));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly construct empty",
            &CSVStringFieldTest::defaultConstructed));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly copy without prior function call",
            &CSVStringFieldTest::copyNoPriorCall));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly copy with prior call",
            &CSVStringFieldTest::copyPriorCall));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly move without prior function call",
            &CSVStringFieldTest::moveNoPriorCall));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVStringFieldTest>(
            "should properly move with prior call",
            &CSVStringFieldTest::movePriorCall));

        return suiteOfTests;
    }

    void escape() {
        yapet::CSVStringField csvStringField{"Test,field"};

        std::string actual{csvStringField.escape()};
        std::string expected{"\"Test,field\""};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"Test\nfield"};

        actual = csvStringField2.escape();
        expected = "\"Test\nfield\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void escapeDoubleQuote() {
        yapet::CSVStringField csvStringField{"Test,\"field\""};

        std::string actual = csvStringField.escape();
        std::string expected{"\"Test,\"\"field\"\"\""};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"Test \"field\""};

        actual = csvStringField2.escape();
        expected = "Test \"field\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField3{",\"\"\""};
        actual = csvStringField3.escape();
        expected = "\",\"\"\"\"\"\"\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void noEscape() {
        yapet::CSVStringField csvStringField{"No escape necessary"};

        std::string actual = csvStringField.escape();
        std::string expected{"No escape necessary"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void escapeEmptyField() {
        yapet::CSVStringField csvStringField{""};

        std::string actual = csvStringField.escape();
        std::string expected{""};

        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void unescape() {
        yapet::CSVStringField csvStringField{"\"test,\"\"field\""};

        std::string actual = csvStringField.unescape();
        std::string expected{"test,\"field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"\"\"\"\""};

        actual = csvStringField2.unescape();
        expected = "\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField3{"\",\"\"\"\"\"\"\""};
        actual = csvStringField3.unescape();
        expected = ",\"\"\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void unescapeWronglyEscapedField() {
        yapet::CSVStringField csvStringField{"\"test,\"field\""};

        CPPUNIT_ASSERT_THROW(csvStringField.unescape(), std::invalid_argument);
    }

    void noUnescape() {
        yapet::CSVStringField csvStringField{"test field"};

        std::string actual = csvStringField.unescape();
        std::string expected{"test field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"test \" field"};

        actual = csvStringField2.unescape();
        expected = "test \" field";
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void unescapeEmptyField() {
        yapet::CSVStringField csvStringField{""};

        std::string actual = csvStringField.unescape();
        std::string expected{""};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void defaultConstructed() {
        yapet::CSVStringField defaultConstructed;
        std::string emptyString{};

        CPPUNIT_ASSERT_EQUAL(emptyString, defaultConstructed.escape());
        CPPUNIT_ASSERT_EQUAL(emptyString, defaultConstructed.unescape());
    }

    void copyNoPriorCall() {
        // Unescape
        yapet::CSVStringField csvStringField{"\"test,\"\"field\""};
        yapet::CSVStringField empty{csvStringField};

        std::string actual = empty.unescape();
        std::string expected{"test,\"field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"\"test,\"\"field\""};
        yapet::CSVStringField empty2{};

        empty2 = csvStringField2;

        actual = empty2.unescape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        // Escape
        yapet::CSVStringField csvStringField3{"Test,field"};
        yapet::CSVStringField empty3{csvStringField3};

        actual = empty3.escape();
        expected = "\"Test,field\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField4{"Test,field"};
        yapet::CSVStringField empty4;

        empty4 = csvStringField4;

        actual = empty4.escape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void copyPriorCall() {
        // Unescape
        yapet::CSVStringField csvStringField{"\"test,\"\"field\""};
        csvStringField.unescape();
        yapet::CSVStringField empty{csvStringField};

        std::string actual = empty.unescape();
        std::string expected{"test,\"field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"\"test,\"\"field\""};
        csvStringField2.unescape();
        yapet::CSVStringField empty2{};

        empty2 = csvStringField2;

        actual = empty2.unescape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        // Escape
        yapet::CSVStringField csvStringField3{"Test,field"};
        csvStringField3.escape();
        yapet::CSVStringField empty3{csvStringField3};

        actual = empty3.escape();
        expected = "\"Test,field\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField4{"Test,field"};
        csvStringField4.escape();
        yapet::CSVStringField empty4;

        empty4 = csvStringField4;

        actual = empty4.escape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void moveNoPriorCall() {
        // Unescape
        yapet::CSVStringField csvStringField{"\"test,\"\"field\""};
        yapet::CSVStringField empty{std::move(csvStringField)};

        std::string actual = empty.unescape();
        std::string expected{"test,\"field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"\"test,\"\"field\""};
        yapet::CSVStringField empty2{};

        empty2 = std::move(csvStringField2);

        actual = empty2.unescape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        // Escape
        yapet::CSVStringField csvStringField3{"Test,field"};
        yapet::CSVStringField empty3{std::move(csvStringField3)};

        actual = empty3.escape();
        expected = "\"Test,field\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField4{"Test,field"};
        yapet::CSVStringField empty4;

        empty4 = std::move(csvStringField4);

        actual = empty4.escape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void movePriorCall() {
        // Unescape
        yapet::CSVStringField csvStringField{"\"test,\"\"field\""};
        csvStringField.unescape();
        yapet::CSVStringField empty{std::move(csvStringField)};

        std::string actual = empty.unescape();
        std::string expected{"test,\"field"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField2{"\"test,\"\"field\""};
        csvStringField2.unescape();
        yapet::CSVStringField empty2{};

        empty2 = std::move(csvStringField2);

        actual = empty2.unescape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        // Escape
        yapet::CSVStringField csvStringField3{"Test,field"};
        csvStringField3.escape();
        yapet::CSVStringField empty3{std::move(csvStringField3)};

        actual = empty3.escape();
        expected = "\"Test,field\"";
        CPPUNIT_ASSERT_EQUAL(expected, actual);

        yapet::CSVStringField csvStringField4{"Test,field"};
        csvStringField4.escape();
        yapet::CSVStringField empty4;

        empty4 = std::move(csvStringField4);

        actual = empty4.escape();
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CSVStringFieldTest::suite());
    return runner.run() ? 0 : 1;
}