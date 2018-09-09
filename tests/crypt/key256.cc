#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include "key256.hh"

constexpr std::uint8_t expected_key[] = {
    0x77, 0xd4, 0x48, 0x04, 0x61, 0xc0, 0x9c, 0x54, 0x33, 0xe1, 0x64,
    0x4a, 0x50, 0x31, 0x15, 0xb7, 0x1f, 0xc1, 0x9d, 0xfe, 0x93, 0xe8,
    0x1a, 0xdb, 0x45, 0x18, 0x24, 0xcf, 0x03, 0xa8, 0xdb, 0x21};

constexpr char password[] = "JustATestPasswordForKeepingSecret";
auto passwordLength =
    static_cast<yapet::SecureArray::size_type>(std::strlen(password));

class Key256Test : public CppUnit::TestFixture {
   private:
    yapet::SecureArray passwordArray{0};

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Key 256");

        suiteOfTests->addTest(new CppUnit::TestCaller<Key256Test>(
            "should create proper key", &Key256Test::testKey));
        suiteOfTests->addTest(new CppUnit::TestCaller<Key256Test>(
            "should create no IV", &Key256Test::testIV));

        return suiteOfTests;
    }

    void setUp() { passwordArray = yapet::toSecureArray(password); }

    void testKey() {
        yapet::Key256 key{};
        key.password(passwordArray);

        for (unsigned int i = 0; i < key.keySize(); i++) {
            CPPUNIT_ASSERT((*key.key())[i] == expected_key[i]);
        }
    }

    void testIV() {
        yapet::Key256 key{};
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
