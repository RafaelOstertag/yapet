#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <stdexcept>

#include "rngengine.hh"

using namespace yapet::pwgen;

class RngEngineTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Random Number Generator Engine Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<RngEngineTest>{
            "RngEngine should return random byte", &RngEngineTest::rngEngine});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngEngineTest>{
            "RngEngine should copy properly", &RngEngineTest::rngEngineCopy});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngEngineTest>{
            "Rng should move properly", &RngEngineTest::rngEngineMove});

        return suiteOfTests;
    }

    void rngEngine() {
        yapet::pwgen::RngEngine rngEngine;
        for (int i = 0; i < 100; i++) {
            rngEngine();
        }
    }

    void rngEngineCopy() {
        yapet::pwgen::RngEngine rngEngine1;

        yapet::pwgen::RngEngine copy;

        for (int i = 0; i < 10; i++) copy();

        yapet::pwgen::RngEngine copy2 = rngEngine1;
        copy2();

        for (int i = 0; i < 10; i++) rngEngine1();
    }

    void rngEngineMove() {
        yapet::pwgen::RngEngine rngEngine1;

        yapet::pwgen::RngEngine moved{std::move(rngEngine1)};

        for (int i = 0; i < 10; i++) moved();
        CPPUNIT_ASSERT_THROW(rngEngine1(), std::runtime_error);

        yapet::pwgen::RngEngine moved2 = std::move(moved);
        for (int i = 0; i < 10; i++) moved2();
        CPPUNIT_ASSERT_THROW(moved(), std::runtime_error);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RngEngineTest::suite());
    return runner.run() ? 0 : 1;
}