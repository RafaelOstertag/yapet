#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <algorithm>

#include "characterpool.hh"

using namespace yapet::pwgen;

class CharacterPoolTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Character Pool Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<CharacterPoolTest>{
            "should properly identify pools", &CharacterPoolTest::arePools});

        suiteOfTests->addTest(new CppUnit::TestCaller<CharacterPoolTest>{
            "should properly return requested pools",
            &CharacterPoolTest::getCharacterPools});

        return suiteOfTests;
    }

    void arePools() {
        CPPUNIT_ASSERT(isLetters(LETTERS));
        CPPUNIT_ASSERT(isDigits(DIGITS));
        CPPUNIT_ASSERT(isPunct(PUNCT));
        CPPUNIT_ASSERT(isSpecial(SPECIAL));
        CPPUNIT_ASSERT(isOther(OTHER));

        CPPUNIT_ASSERT(isLetters(ALL));
        CPPUNIT_ASSERT(isDigits(ALL));
        CPPUNIT_ASSERT(isPunct(ALL));
        CPPUNIT_ASSERT(isSpecial(ALL));
        CPPUNIT_ASSERT(isOther(ALL));

        CPPUNIT_ASSERT(!isLetters(OTHER));
        CPPUNIT_ASSERT(!isDigits(LETTERS));
        CPPUNIT_ASSERT(!isPunct(DIGITS));
        CPPUNIT_ASSERT(!isSpecial(PUNCT));
        CPPUNIT_ASSERT(!isOther(SPECIAL));
    }

    void getCharacterPools() {
        CPPUNIT_ASSERT(letters == getPools(LETTERS));
        CPPUNIT_ASSERT(digits == getPools(DIGITS));
        CPPUNIT_ASSERT(punctuation == getPools(PUNCT));
        CPPUNIT_ASSERT(special == getPools(SPECIAL));
        CPPUNIT_ASSERT(other == getPools(OTHER));

        CPPUNIT_ASSERT((letters + digits + punctuation + special + other) ==
                       getPools(ALL));

        CPPUNIT_ASSERT_THROW(getPools(1 << 8), std::out_of_range);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CharacterPoolTest::suite());
    return runner.run() ? 0 : 1;
}