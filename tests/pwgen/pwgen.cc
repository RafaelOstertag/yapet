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

bool containsCharactersFromPool(const std::string& pool,
                                const yapet::SecureArray& password) {
    for (yapet::SecureArray::size_type i = 0; i < password.size() - 1; i++) {
        char passwordCharacter = static_cast<char>(password[i]);
        if (pool.find(passwordCharacter, 0) != std::string::npos) {
            return true;
        }
    }
    return false;
}

class PasswordGeneratorTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite* suite() {
        CppUnit::TestSuite* suiteOfTests =
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
        PasswordGenerator passwordGenerator{ALL};

        SecureArray password = passwordGenerator.generatePassword(5);

        CPPUNIT_ASSERT(containsCharactersFromPool(letters, password));
        CPPUNIT_ASSERT(containsCharactersFromPool(digits, password));
        CPPUNIT_ASSERT(containsCharactersFromPool(punctuation, password));
        CPPUNIT_ASSERT(containsCharactersFromPool(special, password));
        CPPUNIT_ASSERT(containsCharactersFromPool(other, password));

        CPPUNIT_ASSERT(password.size() == 6);
        CPPUNIT_ASSERT(password[5] == '\0');
    }

    void generateRandomPassword() {
        PasswordGenerator passwordGenerator{ALL};
        SecureArray password1 = passwordGenerator.generatePassword(3);
        SecureArray password2 = passwordGenerator.generatePassword(3);

        CPPUNIT_ASSERT(password1 != password2);
    }

    void generatePasswordSizeOne() {
        PasswordGenerator passwordGenerator{ALL};
        SecureArray password = passwordGenerator.generatePassword(1);

        CPPUNIT_ASSERT(password.size() == 2);
        CPPUNIT_ASSERT(password[1] == '\0');
    }

    void invalidPasswordSize() {
        PasswordGenerator passwordGenerator{ALL};
        CPPUNIT_ASSERT_THROW(passwordGenerator.generatePassword(0),
                             std::invalid_argument);
        CPPUNIT_ASSERT_THROW(passwordGenerator.generatePassword(-1),
                             std::invalid_argument);
    }

    void usePoolsProperly() {
        PasswordGenerator passwordGenerator{LETTERS};
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