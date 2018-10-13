#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "header10.hh"
#include "headererror.hh"

class HeaderVersionTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Header Version");

        suiteOfTests->addTest(new CppUnit::TestCaller<HeaderVersionTest>(
            "int to header version", &HeaderVersionTest::intToHeaderVersion));

        return suiteOfTests;
    }

    void intToHeaderVersion() {
        CPPUNIT_ASSERT(yapet::intToHeaderVersion(1) ==
                       yapet::HEADER_VERSION::VERSION_1);
        CPPUNIT_ASSERT(yapet::intToHeaderVersion(2) ==
                       yapet::HEADER_VERSION::VERSION_2);

        CPPUNIT_ASSERT_THROW(yapet::intToHeaderVersion(0), yapet::HeaderError);
        CPPUNIT_ASSERT_THROW(yapet::intToHeaderVersion(3), yapet::HeaderError);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(HeaderVersionTest::suite());
    return runner.run() ? 0 : 1;
}