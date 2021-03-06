#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "header10.hh"
#include "headererror.hh"

class Header10Test : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Header 1.0");

        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should serialize and deserialize with constructor",
            &Header10Test::serializeAndDeserializeWithCtor));
        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should serialize and deserialize by assignment",
            &Header10Test::serializeAndDeserializeWithAssignment));
        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should throw on deserializing invalid header",
            &Header10Test::invalidHeader));

        return suiteOfTests;
    }

    void serializeAndDeserializeWithCtor() {
        yapet::Header10 header10{};

        yapet::SecureArray serialized{header10.serialize()};

        yapet::Header10 fromSerializedHeader{serialized};

        CPPUNIT_ASSERT(header10.passwordSetTime() ==
                       fromSerializedHeader.passwordSetTime());
        CPPUNIT_ASSERT(header10.version() == fromSerializedHeader.version());
    }

    void serializeAndDeserializeWithAssignment() {
        yapet::Header10 header10{};

        yapet::SecureArray serialized = header10.serialize();

        yapet::Header10 fromSerializedHeader{serialized};

        CPPUNIT_ASSERT(header10.passwordSetTime() ==
                       fromSerializedHeader.passwordSetTime());
        CPPUNIT_ASSERT(header10.version() == fromSerializedHeader.version());
    }

    void invalidHeader() {
        yapet::Header10 invalidHeader{std::time(0), 3};

        yapet::SecureArray serialized{invalidHeader.serialize()};

        yapet::Header10 deserializedHeader{};
        CPPUNIT_ASSERT_THROW(deserializedHeader = serialized,
                             yapet::HeaderError);

        yapet::Header10 header{};

        serialized = header.serialize();
        (*serialized)[4] = 'Z';

        CPPUNIT_ASSERT_THROW(deserializedHeader = serialized,
                             yapet::ControlStringMismatch);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Header10Test::suite());
    return runner.run() ? 0 : 1;
}