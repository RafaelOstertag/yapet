#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>
#include <typeinfo>

#include "aes256factory.hh"
#include "blowfishfactory.hh"
#include "cryptofactoryhelper.hh"
#include "testpaths.h"

class CryptoFactoryHelperTest : public CppUnit::TestFixture {
   private:
    yapet::SecureArray passwordArray{0};

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Crypto Factory Helper Test");

        suiteOfTests->addTest(new CppUnit::TestCaller<CryptoFactoryHelperTest>(
            "should create blowfish factory",
            &CryptoFactoryHelperTest::yapet10file));
        suiteOfTests->addTest(new CppUnit::TestCaller<CryptoFactoryHelperTest>(
            "should create AES256 factory",
            &CryptoFactoryHelperTest::yapet20file));
        suiteOfTests->addTest(new CppUnit::TestCaller<CryptoFactoryHelperTest>(
            "should gracefully handle unkown file",
            &CryptoFactoryHelperTest::unknownFile));
        suiteOfTests->addTest(new CppUnit::TestCaller<CryptoFactoryHelperTest>(
            "should gracefully handle error file",
            &CryptoFactoryHelperTest::errorFile));

        return suiteOfTests;
    }

    void yapet10file() {
        auto factory{yapet::getCryptoFactoryForFile(
            SRCDIR "/cryptofactoryhelper-1.0.pet",
            yapet::toSecureArray("wdc"))};

        CPPUNIT_ASSERT(typeid(*factory) == typeid(yapet::BlowfishFactory));
    }

    void yapet20file() {
        auto factory{yapet::getCryptoFactoryForFile(
            SRCDIR "/cryptofactoryhelper-2.0.pet",
            yapet::toSecureArray("wdc"))};

        CPPUNIT_ASSERT(typeid(*factory) == typeid(yapet::Aes256Factory));
    }

    void unknownFile() {
        auto factory{yapet::getCryptoFactoryForFile(
            SRCDIR "/cryptofactoryhelper-unknown.pet",
            yapet::toSecureArray("wdc"))};

        CPPUNIT_ASSERT(!factory);
    }

    void errorFile() {
        auto factory{yapet::getCryptoFactoryForFile(
            SRCDIR "/cryptofactoryhelper-tooshort.pet",
            yapet::toSecureArray("wdc"))};

        CPPUNIT_ASSERT(!factory);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CryptoFactoryHelperTest::suite());
    return runner.run() ? 0 : 1;
}
