#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "blowfish.hh"
#include "key448.hh"

class Header10Test : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::Blowfish> blowfish;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Blowfish");

        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should encrypt and decrypt", &Header10Test::encryptDecrypt));

        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should throw on decrypting corrupted data",
            &Header10Test::decryptCorruptData));

        suiteOfTests->addTest(new CppUnit::TestCaller<Header10Test>(
            "should throw on empty plain/cipher text",
            &Header10Test::throwOnEmptyPlainAndCipherText));

        return suiteOfTests;
    }

    void setUp() {
        std::shared_ptr<yapet::Key> key{new yapet::Key448{}};
        key->password(yapet::toSecureArray("test"));

        blowfish = std::unique_ptr<yapet::Blowfish>{new yapet::Blowfish{key}};
    }

    void encryptDecrypt() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = blowfish->encrypt(plainText);
        auto actual = blowfish->decrypt(cipherText);

        CPPUNIT_ASSERT(plainText == actual);
    }

    void decryptCorruptData() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = blowfish->encrypt(plainText);

        yapet::SecureArray corrupt{cipherText.size() - 2};
        corrupt << cipherText;

        CPPUNIT_ASSERT_THROW(blowfish->decrypt(corrupt),
                             YAPET::YAPETEncryptionException);
    }

    void throwOnEmptyPlainAndCipherText() {
        yapet::SecureArray empty{};

        CPPUNIT_ASSERT_THROW(blowfish->encrypt(empty), YAPET::YAPETException);
        CPPUNIT_ASSERT_THROW(blowfish->decrypt(empty), YAPET::YAPETException);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Header10Test::suite());
    return runner.run() ? 0 : 1;
}