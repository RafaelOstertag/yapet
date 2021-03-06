#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "csvline.hh"

class CSVLineTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("CSV Line");

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should produce correct line on empty fields",
            &CSVLineTest::getEmptyLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should return fields unescaped",
            &CSVLineTest::getFieldsUnescaped));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should produce correct line", &CSVLineTest::getLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should throw on adding to much fields",
            &CSVLineTest::throwTooMuchFields));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should throw on getting invalid index",
            &CSVLineTest::throwInvalidIndex));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should parse line correctly", &CSVLineTest::parseLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should parse corner case lines correctly",
            &CSVLineTest::parseCornerCases));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should parse empty CSV line correctly",
            &CSVLineTest::parseEmptyCsvLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should throw on empty line", &CSVLineTest::throwEmptyLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should throw on malformed line",
            &CSVLineTest::throwMalformedLine));

        suiteOfTests->addTest(new CppUnit::TestCaller<CSVLineTest>(
            "should use different separator", &CSVLineTest::separator));

        return suiteOfTests;
    }

    void getEmptyLine() {
        yapet::CSVLine csvLine{3};

        std::string actual{csvLine.getLine()};
        std::string expected{",,"};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void getFieldsUnescaped() {
        yapet::CSVLine csvLine{1};

        csvLine.addField(0, "\",\"");

        std::string actual{csvLine[0]};
        std::string expected{","};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void getLine() {
        yapet::CSVLine csvLine{3};

        csvLine.addField(0, "Field 1");
        csvLine.addField(1, "Field,2");
        csvLine.addField(2, "Field\",3");

        std::string actual{csvLine.getLine()};
        std::string expected{"Field 1,\"Field,2\",\"Field\"\",3\""};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }

    void throwTooMuchFields() {
        yapet::CSVLine csvLine{2};

        CPPUNIT_ASSERT_THROW(csvLine.addField(3, ""), std::out_of_range);
        CPPUNIT_ASSERT_THROW(csvLine[3], std::out_of_range);
    }

    void throwInvalidIndex() {
        yapet::CSVLine csvLine{2};
        CPPUNIT_ASSERT_THROW(csvLine[3], std::out_of_range);
    }

    void parseLine() {
        yapet::CSVLine csvLine1{3};
        csvLine1.parseLine("a,b,c");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine1[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b"}, csvLine1[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"c"}, csvLine1[2]);

        yapet::CSVLine csvLine2{1};
        csvLine2.parseLine("a");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine2[0]);

        yapet::CSVLine csvLine3{2};
        CPPUNIT_ASSERT_THROW(csvLine3.parseLine("a,b,c"),
                             std::invalid_argument);

        yapet::CSVLine csvLine4{2};
        CPPUNIT_ASSERT_THROW(csvLine4.parseLine("a"), std::invalid_argument);
    }

    void parseCornerCases() {
        yapet::CSVLine csvLine2{2};
        csvLine2.parseLine("\"a,\",b\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a,"}, csvLine2[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b\""}, csvLine2[1]);

        yapet::CSVLine csvLine3{3};
        csvLine3.parseLine("a,b\",");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b\""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        csvLine3.parseLine("a,\"b,\"\"\",");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b,\""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        // Start of line
        csvLine3.parseLine("\"\"\"\",a,");
        CPPUNIT_ASSERT_EQUAL(std::string{"\""}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        csvLine3.parseLine("\",\"\"\",a,");
        CPPUNIT_ASSERT_EQUAL(std::string{",\""}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        csvLine3.parseLine("\"\"\",\",a,");
        CPPUNIT_ASSERT_EQUAL(std::string{"\","}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        // In middle of line
        csvLine3.parseLine("a,\"\"\"\",");
        CPPUNIT_ASSERT_EQUAL(std::string{"\""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        csvLine3.parseLine("a,\",\"\"\",");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{",\""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        csvLine3.parseLine("a,\"\"\",\",");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"\","}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine3[2]);

        // at end of line
        csvLine3.parseLine("a,b,\"\"\"\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"\""}, csvLine3[2]);

        csvLine3.parseLine("a,b,\",\"\"\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{",\""}, csvLine3[2]);

        csvLine3.parseLine("a,,\"\"\",\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"\","}, csvLine3[2]);

        csvLine3.parseLine("a,\"b\",\"c\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b"}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"c"}, csvLine3[2]);

        csvLine2.parseLine("a\",b");
        CPPUNIT_ASSERT_EQUAL(std::string{"a\""}, csvLine2[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b"}, csvLine2[1]);

        csvLine2.parseLine("a,\"b\"\",c\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine2[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b\",c"}, csvLine2[1]);

        csvLine2.parseLine("a,\"\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine2[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{""}, csvLine2[1]);

        csvLine3.parseLine("a,\"\",\",\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{","}, csvLine3[2]);

        csvLine3.parseLine("a,\"\"\"\",\",\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a"}, csvLine3[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"\""}, csvLine3[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{","}, csvLine3[2]);
    }

    void parseEmptyCsvLine() {
        yapet::CSVLine csvLine{3};
        csvLine.parseLine(",,");

        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{}, csvLine[2]);
    }

    void throwEmptyLine() {
        yapet::CSVLine csvLine{3};
        CPPUNIT_ASSERT_THROW(csvLine.parseLine(""), std::invalid_argument);
    }

    void throwMalformedLine() {
        yapet::CSVLine csvLine1{3};
        CPPUNIT_ASSERT_THROW(csvLine1.parseLine("a,\"b,c"),
                             std::invalid_argument);
    }

    void separator() {
        yapet::CSVLine csvLine1{4, ';'};
        csvLine1.parseLine("\"a;\";b\"\";\"c;\"\"\";\"d\"");
        CPPUNIT_ASSERT_EQUAL(std::string{"a;"}, csvLine1[0]);
        CPPUNIT_ASSERT_EQUAL(std::string{"b\"\""}, csvLine1[1]);
        CPPUNIT_ASSERT_EQUAL(std::string{"c;\""}, csvLine1[2]);
        CPPUNIT_ASSERT_EQUAL(std::string{"d"}, csvLine1[3]);

        yapet::CSVLine csvLine2{3, ';'};
        csvLine2.addField(0, "Field 1");
        csvLine2.addField(1, "Field;2");
        csvLine2.addField(2, "Field\";3");

        std::string actual{csvLine2.getLine()};
        std::string expected{"Field 1;\"Field;2\";\"Field\"\";3\""};
        CPPUNIT_ASSERT_EQUAL(expected, actual);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CSVLineTest::suite());
    return runner.run() ? 0 : 1;
}