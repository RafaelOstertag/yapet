#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>
#include <stdexcept>

#include "pwgen.hh"

using namespace yapet;
using namespace yapet::pwgen;

class PasswordGeneratorTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Random Number Generator Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordGeneratorTest>{
            "should properly generate password",
            &PasswordGeneratorTest::generatePassword});

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordGeneratorTest>{
            "should generate random password",
            &PasswordGeneratorTest::generateRandomPassword});

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordGeneratorTest>{
            "should handle size 1 password",
            &PasswordGeneratorTest::generatePasswordSizeOne});

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordGeneratorTest>{
            "should throw on invalid password size",
            &PasswordGeneratorTest::invalidPasswordSize});

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordGeneratorTest>{
            "should use pools properly",
            &PasswordGeneratorTest::usePoolsProperly});

        return suiteOfTests;
    }

    void generatePassword() {
        PasswordGenerator passwordGenerator{RAND, ALL};
        SecureArray password = passwordGenerator.generatePassword(3);

        CPPUNIT_ASSERT(password.size() == 4);
        CPPUNIT_ASSERT(password[3] == '\0');
    }

    void generateRandomPassword() {
        PasswordGenerator passwordGenerator{DEVURANDOM, ALL};
        SecureArray password1 = passwordGenerator.generatePassword(3);
        SecureArray password2 = passwordGenerator.generatePassword(3);

        CPPUNIT_ASSERT(password1 != password2);

        passwordGenerator.rngEngine(RAND);
        password1 = passwordGenerator.generatePassword(3);
        password2 = passwordGenerator.generatePassword(3);

        CPPUNIT_ASSERT(password1 != password2);
    }

    void generatePasswordSizeOne() {
        PasswordGenerator passwordGenerator{RAND, ALL};
        SecureArray password = passwordGenerator.generatePassword(1);

        CPPUNIT_ASSERT(password.size() == 2);
        CPPUNIT_ASSERT(password[1] == '\0');
    }

    void invalidPasswordSize() {
        PasswordGenerator passwordGenerator{RAND, ALL};
        CPPUNIT_ASSERT_THROW(passwordGenerator.generatePassword(0),
                             std::invalid_argument);
        CPPUNIT_ASSERT_THROW(passwordGenerator.generatePassword(-1),
                             std::invalid_argument);
    }

    void usePoolsProperly() {
        PasswordGenerator passwordGenerator{RAND, LETTERS};
        SecureArray password{passwordGenerator.generatePassword(5)};

        for (SecureArray::size_type i = 0; i < password.size() - 1; i++) {
            CPPUNIT_ASSERT(std::isalpha(password[i]));
        }

        passwordGenerator.characterPools(DIGITS);
        password = passwordGenerator.generatePassword(5);

        for (SecureArray::size_type i = 0; i < password.size() - 1; i++) {
            CPPUNIT_ASSERT(std::isdigit(password[i]));
        }
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(PasswordGeneratorTest::suite());
    return runner.run() ? 0 : 1;
}