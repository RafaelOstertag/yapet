#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include "consts.h"
#include "key256.hh"

constexpr std::uint8_t expected_key[] = {
    0x85, 0x9c, 0xb0, 0x9e, 0xa2, 0x17, 0xb7, 0xb6, 0x6b, 0xd6, 0x2f,
    0x08, 0xa1, 0xcd, 0x44, 0xcb, 0xb6, 0x8a, 0xdd, 0x4a, 0x55, 0x17,
    0x1e, 0xbb, 0xa6, 0xe1, 0xe9, 0xbe, 0x06, 0x9a, 0x46, 0xab,
};

constexpr char password[] = "JustATestPasswordForKeepingSecret";
auto passwordLength =
    static_cast<yapet::SecureArray::size_type>(std::strlen(password));

class Key256Test : public CppUnit::TestFixture {
   private:
    yapet::SecureArray passwordArray{0};
    yapet::MetaData parameters;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Key 256");

        suiteOfTests->addTest(new CppUnit::TestCaller<Key256Test>(
            "should create proper key", &Key256Test::testKey));
        suiteOfTests->addTest(new CppUnit::TestCaller<Key256Test>(
            "should create no IV", &Key256Test::testIV));

        return suiteOfTests;
    }

    void setUp() {
        passwordArray = yapet::toSecureArray(password);
        yapet::MetaData p;
        p.setValue(YAPET::Consts::ARGON2_MEMORY_COST_KEY, 262144);
        p.setValue(YAPET::Consts::ARGON2_PARALLELISM_KEY, 16);
        p.setValue(YAPET::Consts::ARGON2_TIME_COST_KEY, 5);
        p.setValue(YAPET::Consts::ARGON2_SALT1_KEY, 0x61626364);
        p.setValue(YAPET::Consts::ARGON2_SALT2_KEY, 0x65666768);
        p.setValue(YAPET::Consts::ARGON2_SALT3_KEY, 0x696a6b6c);
        p.setValue(YAPET::Consts::ARGON2_SALT4_KEY, 0x6d6e6f70);

        parameters = p;
    }

    void testKey() {
        yapet::Key256 key{};
        key.keyingParameters(parameters);
        key.password(passwordArray);

        for (int i = 0; i < key.keySize(); i++) {
            CPPUNIT_ASSERT((*key.key())[i] == expected_key[i]);
        }
    }

    void testIV() {
        yapet::Key256 key{};
        key.keyingParameters(parameters);
        key.password(passwordArray);

        CPPUNIT_ASSERT(key.ivecSize() == 0);
        CPPUNIT_ASSERT(key.ivec() == yapet::SecureArray{});
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Key256Test::suite());
    return runner.run() ? 0 : 1;
}
