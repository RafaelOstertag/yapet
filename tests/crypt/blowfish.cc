#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "blowfish.h"

class Header10Test : public CppUnit::TestFixture {
   private:
    yapet::SecureArray passwordArray{0};

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Key");

        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should create proper key", &Header10Test::testKey));
        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should create proper IV", &Header10Test::testIV));

        return suiteOfTests;
    }

    void setUp() {
        passwordArray = yapet::SecureArray{passwordLength};
        std::memcpy(*passwordArray, password, passwordLength);
    }

    void testKey() {
        YAPET::Key448 key(passwordArray);

        for (unsigned int i = 0; i < key.keySize(); i++) {
            CPPUNIT_ASSERT((*key.key())[i] == expected_key[i]);
        }
    }

    void testIV() {
        YAPET::Key448 key(passwordArray);

        for (unsigned int i = 0; i < key.ivecSize(); i++) {
            CPPUNIT_ASSERT((*key.ivec())[i] == expected_ivec[i]);
        }
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Header10Test::suite());
    return runner.run() ? 0 : 1;
}