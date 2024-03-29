#include "key448.hh"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include "openssl.hh"

constexpr std::uint8_t expected_key[] = {
    0x3e, 0xc3, 0x34, 0x5d, 0x72, 0x83, 0xbd, 0x09, 0x60, 0xa3, 0x4f, 0x6b,
    0x59, 0x5b, 0x93, 0xb6, 0x66, 0x91, 0x15, 0x2c, 0x65, 0xc8, 0x1a, 0xdc,
    0x0a, 0xc9, 0x90, 0xa7, 0x93, 0x70, 0x96, 0x94, 0xa1, 0x6d, 0x81, 0x06,
    0xc1, 0x9a, 0xba, 0xd0, 0x0e, 0x94, 0xd0, 0xd1, 0xf3, 0xeb, 0x5d, 0x4d,
    0xd8, 0xd1, 0x95, 0x72, 0xac, 0x33, 0x86, 0xe0};

constexpr std::uint8_t expected_ivec[] = {0xb2, 0x19, 0x22, 0x33,
                                          0x6d, 0x96, 0x15, 0x85};

constexpr char password[] = "JustATestPasswordForKeepingSecret";
auto passwordLength =
    static_cast<yapet::SecureArray::size_type>(std::strlen(password));

class Key448Test : public CppUnit::TestFixture {
   private:
    yapet::SecureArray passwordArray{0};

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Key 448");

        suiteOfTests->addTest(new CppUnit::TestCaller<Key448Test>(
            "should create proper key", &Key448Test::testKey));
        suiteOfTests->addTest(new CppUnit::TestCaller<Key448Test>(
            "should create proper IV", &Key448Test::testIV));

        suiteOfTests->addTest(new CppUnit::TestCaller<Key448Test>(
            "should return empty key parameters",
            &Key448Test::emtpyKeyParameters));

        return suiteOfTests;
    }

    void setUp() { passwordArray = yapet::toSecureArray(password); }

    void testKey() {
        yapet::Key448 key{};
        key.password(passwordArray);

        for (int i = 0; i < key.keySize(); i++) {
            CPPUNIT_ASSERT((*key.key())[i] == expected_key[i]);
        }
    }

    void testIV() {
        yapet::Key448 key{};
        key.password(passwordArray);

        for (int i = 0; i < key.ivecSize(); i++) {
            CPPUNIT_ASSERT((*key.ivec())[i] == expected_ivec[i]);
        }
    }

    void emtpyKeyParameters() {
        yapet::Key448 key{};

        CPPUNIT_ASSERT(key.keyingParameters().size() == 0);
    }
};

int main() {
    yapet::OpenSSL::init();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Key448Test::suite());
    return runner.run() ? 0 : 1;
}
