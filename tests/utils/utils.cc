#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "utils.hh"

class UtilsTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Utils Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<UtilsTest>(
            "deleteAndZero should free mem", &UtilsTest::deleteAndZero));

        return suiteOfTests;
    }

    void deleteAndZero() {
        // Must not segfault
        yapet::deleteAndZero((int **)nullptr);

        int *ptr = nullptr;
        // must not segfault
        yapet::deleteAndZero(&ptr);

        ptr = new int;
        // must not segfault
        yapet::deleteAndZero(&ptr);

        CPPUNIT_ASSERT(ptr == nullptr);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(UtilsTest::suite());
    return runner.run() ? 0 : 1;
}
