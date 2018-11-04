#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "aes256.hh"
#include "consts.h"
#include "cryptoerror.hh"
#include "key256.hh"

class Aes256Test : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::Aes256> aes256;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("AES 256");

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256Test>(
            "should encrypt and decrypt", &Aes256Test::encryptDecrypt));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256Test>(
            "should throw on decrypting corrupted data",
            &Aes256Test::decryptCorruptData));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256Test>(
            "should throw on decrypting with wrong password",
            &Aes256Test::decryptWithWrongPassword));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256Test>(
            "should throw on empty plain/cipher text",
            &Aes256Test::throwOnEmptyPlainAndCipherText));

        return suiteOfTests;
    }

    yapet::MetaData keyingParameters1() {
        yapet::MetaData metaData{};
        metaData.setValue(YAPET::Consts::ARGON2_MEMORY_COST_KEY, 65000);
        metaData.setValue(YAPET::Consts::ARGON2_PARALLELISM_KEY, 1);
        metaData.setValue(YAPET::Consts::ARGON2_TIME_COST_KEY, 1);
        metaData.setValue(YAPET::Consts::ARGON2_SALT1_KEY, 0x1234);
        metaData.setValue(YAPET::Consts::ARGON2_SALT2_KEY, 0x5678);
        metaData.setValue(YAPET::Consts::ARGON2_SALT3_KEY, 0x9ABC);
        metaData.setValue(YAPET::Consts::ARGON2_SALT4_KEY, 0xDEF0);

        return metaData;
    }

    void setUp() {
        std::shared_ptr<yapet::Key> key{new yapet::Key256{}};
        key->keyingParameters(keyingParameters1());
        key->password(yapet::toSecureArray("test"));

        aes256 = std::unique_ptr<yapet::Aes256>{new yapet::Aes256{key}};
    }

    void encryptDecrypt() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = aes256->encrypt(plainText);
        auto actual = aes256->decrypt(cipherText);

        CPPUNIT_ASSERT(plainText == actual);
    }

    void decryptCorruptData() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = aes256->encrypt(plainText);

        yapet::SecureArray corrupt{cipherText.size() - 2};
        corrupt << cipherText;

        CPPUNIT_ASSERT_THROW(aes256->decrypt(corrupt), yapet::EncryptionError);
    }

    void decryptWithWrongPassword() {
        auto plainText{yapet::toSecureArray("Encryption test")};
        auto cipherText = aes256->encrypt(plainText);

        std::shared_ptr<yapet::Key> otherKey{new yapet::Key256{}};
        otherKey->keyingParameters(keyingParameters1());
        otherKey->password(yapet::toSecureArray("invalid"));

        auto otherAes256{
            std::unique_ptr<yapet::Aes256>{new yapet::Aes256{otherKey}}};

        CPPUNIT_ASSERT_THROW(otherAes256->decrypt(cipherText),
                             yapet::EncryptionError);
    }

    void throwOnEmptyPlainAndCipherText() {
        yapet::SecureArray empty{};

        CPPUNIT_ASSERT_THROW(aes256->encrypt(empty), yapet::EncryptionError);
        CPPUNIT_ASSERT_THROW(aes256->decrypt(empty), yapet::EncryptionError);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Aes256Test::suite());
    return runner.run() ? 0 : 1;
}