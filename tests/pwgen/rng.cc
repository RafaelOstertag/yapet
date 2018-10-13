#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <stdexcept>

#include "rng.hh"

using namespace yapet::pwgen;

constexpr std::uint8_t HI{5};

class RngTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Random Number Generator Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "Rng should return random byte", &RngTest::rng});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "Rng should copy properly", &RngTest::rngCopy});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "Rng should copy properly", &RngTest::rngMove});

        return suiteOfTests;
    }

    void rng() {
        yapet::pwgen::Rng rng{ HI};
        for (int i = 0; i < 100; i++) {
            CPPUNIT_ASSERT(rng.getNextInt() <= HI);
        }
    }

    void rngCopy() {
        yapet::pwgen::Rng rng1{ HI};

        yapet::pwgen::Rng copy{rng1};

        copy.getNextInt();

        yapet::pwgen::Rng copy2 = rng1;
        copy2.getNextInt();

        rng1.getNextInt();
    }

    void rngMove() {
        yapet::pwgen::Rng rng1{ HI};

        yapet::pwgen::Rng moved{std::move(rng1)};

        moved.getNextInt();
        CPPUNIT_ASSERT_THROW(rng1.getNextInt(), std::runtime_error);

        yapet::pwgen::Rng moved2 = std::move(moved);
        moved2.getNextInt();
        CPPUNIT_ASSERT_THROW(moved.getNextInt(), std::runtime_error);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RngTest::suite());
    return runner.run() ? 0 : 1;
}