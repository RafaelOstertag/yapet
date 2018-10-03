#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <stdexcept>

#include "rng.hh"

using namespace yapet::pwgen;

class RngTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Random Number Generator Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "RngFile should return random byte", &RngTest::rngFile});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "RngFile should copy properly", &RngTest::rngFileCopy});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "RngFile should copy properly", &RngTest::rngFileMove});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "RngRand should return random byte", &RngTest::rngRand});

        suiteOfTests->addTest(new CppUnit::TestCaller<RngTest>{
            "getRng should return proper RNG", &RngTest::getRng});

        return suiteOfTests;
    }

    void rngFile() {
        yapet::pwgen::RngFile rngFile{"/dev/urandom"};
        rngFile.getNextByte();
    }

    void rngFileCopy() {
        yapet::pwgen::RngFile rngFile1{"/dev/urandom"};

        yapet::pwgen::RngFile copy{rngFile1};

        copy.getNextByte();

        yapet::pwgen::RngFile copy2 = rngFile1;
        copy2.getNextByte();

        rngFile1.getNextByte();
    }

    void rngFileMove() {
        yapet::pwgen::RngFile rngFile1{"/dev/urandom"};

        yapet::pwgen::RngFile moved{std::move(rngFile1)};

        moved.getNextByte();
        CPPUNIT_ASSERT_THROW(rngFile1.getNextByte(), std::runtime_error);

        yapet::pwgen::RngFile moved2 = std::move(moved);
        moved2.getNextByte();
        CPPUNIT_ASSERT_THROW(moved.getNextByte(), std::runtime_error);
    }

    void rngRand() {
        yapet::pwgen::RngRand rngRand{};

        rngRand.getNextByte();
    }

    void getRng() {
        std::unique_ptr<RngInterface> rng{yapet::pwgen::getRng(RAND)};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*rng) == typeid(yapet::pwgen::RngRand));
#pragma clang diagnostic pop

        rng = yapet::pwgen::getRng(DEVRANDOM);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*rng) == typeid(yapet::pwgen::RngFile));
#pragma clang diagnostic pop

        const yapet::pwgen::RngFile *ptr =
            dynamic_cast<const yapet::pwgen::RngFile *>(rng.get());
        CPPUNIT_ASSERT_EQUAL(std::string{"/dev/random"}, ptr->filename());

        rng = yapet::pwgen::getRng(DEVURANDOM);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*rng) == typeid(yapet::pwgen::RngFile));
#pragma clang diagnostic pop
        ptr = dynamic_cast<const yapet::pwgen::RngFile *>(rng.get());
        CPPUNIT_ASSERT_EQUAL(std::string{"/dev/urandom"}, ptr->filename());
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(RngTest::suite());
    return runner.run() ? 0 : 1;
}