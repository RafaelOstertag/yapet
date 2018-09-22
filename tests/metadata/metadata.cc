#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "metadata.hh"

class MetaDataTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Meta Data Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<MetaDataTest>(
            "should set and get meta data", &MetaDataTest::basic));

        suiteOfTests->addTest(new CppUnit::TestCaller<MetaDataTest>(
            "should throw on non-existing key",
            &MetaDataTest::throwOnNonExistingKey));

        suiteOfTests->addTest(new CppUnit::TestCaller<MetaDataTest>(
            "should deserialize empty SecureArray",
            &MetaDataTest::serializeEmptySecureArray));

        suiteOfTests->addTest(new CppUnit::TestCaller<MetaDataTest>(
            "should throw on deserializing invalid SecureArray",
            &MetaDataTest::throwOnInvalidSecureArray));

        suiteOfTests->addTest(new CppUnit::TestCaller<MetaDataTest>(
            "should serialize and deserialize",
            &MetaDataTest::serializeAndDeserialize));
        return suiteOfTests;
    }

    void basic() {
        yapet::MetaData metaData;

        metaData.setValue("test", 42);
        CPPUNIT_ASSERT(metaData.getValue("test") == 42);
    }

    void throwOnNonExistingKey() {
        yapet::MetaData metaData;

        CPPUNIT_ASSERT_THROW(metaData.getValue("non-existing"),
                             std::out_of_range);
    }

    void serializeEmptySecureArray() {
        yapet::MetaData metaData{yapet::SecureArray{}};
    }

    void throwOnInvalidSecureArray() {
        yapet::SecureArray secureArray{yapet::toSecureArray("test")};

        CPPUNIT_ASSERT_THROW(yapet::MetaData{secureArray}, std::invalid_argument);
    }

    void serializeAndDeserialize() {
        yapet::MetaData metaData;
        metaData.setValue("a", 42);
        metaData.setValue("longerName", 5);
        metaData.setValue("b", 3);

        auto serialized{metaData.serialize()};

        yapet::MetaData deserialized{serialized};
        CPPUNIT_ASSERT(deserialized.getValue("a") == 42);
        CPPUNIT_ASSERT(deserialized.getValue("b") == 3);
        CPPUNIT_ASSERT(deserialized.getValue("longerName") == 5);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(MetaDataTest::suite());
    return runner.run() ? 0 : 1;
}
