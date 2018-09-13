#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "config.h"

#include "ods.hh"

class ODSTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("On Disk Structure");

        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "16bits integer to On-Disk-Structure", &ODSTest::toODS16));
        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "16bits integer to host", &ODSTest::toHost16));

        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "32bits integer to On-Disk-Structure", &ODSTest::toODS32));
        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "32bits integer to host", &ODSTest::toHost32));

        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "64bits integer to On-Disk-Structure", &ODSTest::toODS64));
        suiteOfTests->addTest(new CppUnit::TestCaller<ODSTest>(
            "64bits integer to host", &ODSTest::toHost64));
        return suiteOfTests;
    }

    void toODS16() {
        std::uint16_t subject = 0x0a0b;
#ifdef WORDS_BIGENDIAN
        std::uint16_t expected = 0x0a0b;
#else
        std::uint16_t expected = 0x0b0a;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toODS(subject));
    }

    void toHost16() {
        std::uint16_t subject = 0x0a0b;
#ifdef WORDS_BIGENDIAN
        std::uint16_t expected = 0x0a0b;
#else
        std::uint16_t expected = 0x0b0a;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toHost(subject));
    }

    void toODS32() {
        std::uint32_t subject = 0x0a0b0c0d;
#ifdef WORDS_BIGENDIAN
        std::uint32_t expected = 0x0a0b0c0d;
#else
        std::uint32_t expected = 0x0d0c0b0a;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toODS(subject));
    }

    void toHost32() {
        std::uint32_t subject = 0x0a0b0c0d;
#ifdef WORDS_BIGENDIAN
        std::uint32_t expected = 0x0a0b0c0d;
#else
        std::uint32_t expected = 0x0d0c0b0a;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toHost(subject));
    }

    void toODS64() {
        std::uint64_t subject = 0x0102030405060708;
#ifdef WORDS_BIGENDIAN
        std::uint64_t expected = 0x0102030405060708;
#else
        std::uint64_t expected = 0x0807060504030201;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toODS(subject));
    }

    void toHost64() {
        std::uint64_t subject = 0x0102030405060708;
#ifdef WORDS_BIGENDIAN
        std::uint64_t expected = 0x0102030405060708;
#else
        std::uint64_t expected = 0x0807060504030201;
#endif
        CPPUNIT_ASSERT_EQUAL(expected, yapet::toHost(subject));
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(ODSTest::suite());
    return runner.run() ? 0 : 1;
}
