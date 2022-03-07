#include "blowfish.hh"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cryptoerror.hh"
#include "key448.hh"
#include "openssl.hh"

class BlowfishTest : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::Blowfish> blowfish;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Blowfish");

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishTest>(
            "should encrypt and decrypt", &BlowfishTest::encryptDecrypt));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishTest>(
            "should throw on decrypting corrupted data",
            &BlowfishTest::decryptCorruptData));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishTest>(
            "should throw on decrypting with wrong password",
            &BlowfishTest::decryptWithWrongPassword));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishTest>(
            "should throw on empty plain/cipher text",
            &BlowfishTest::throwOnEmptyPlainAndCipherText));

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
                             yapet::EncryptionError);
    }

    void decryptWithWrongPassword() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = blowfish->encrypt(plainText);

        std::shared_ptr<yapet::Key> otherKey{new yapet::Key448{}};
        otherKey->password(yapet::toSecureArray("invalid"));

        auto otherBlowfish{
            std::unique_ptr<yapet::Blowfish>{new yapet::Blowfish{otherKey}}};

        CPPUNIT_ASSERT_THROW(otherBlowfish->decrypt(cipherText),
                             yapet::EncryptionError);
    }

    void throwOnEmptyPlainAndCipherText() {
        yapet::SecureArray empty{};

        CPPUNIT_ASSERT_THROW(blowfish->encrypt(empty), yapet::EncryptionError);
        CPPUNIT_ASSERT_THROW(blowfish->decrypt(empty), yapet::EncryptionError);
    }
};

int main() {
    yapet::OpenSSL::init();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(BlowfishTest::suite());
    return runner.run() ? 0 : 1;
}