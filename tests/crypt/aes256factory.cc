#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "aes256factory.hh"
#include "key256.hh"
#include "testpaths.h"

class Aes256FactoryTest : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::Aes256Factory> aes256Factory;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("AES 256 Factory");

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256FactoryTest>(
            "should give correct crypto", &Aes256FactoryTest::crypto));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256FactoryTest>(
            "should give correct key", &Aes256FactoryTest::key));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256FactoryTest>(
            "should give correct file", &Aes256FactoryTest::file));

        suiteOfTests->addTest(new CppUnit::TestCaller<Aes256FactoryTest>(
            "should create proper new factory",
            &Aes256FactoryTest::newFactory));

        return suiteOfTests;
    }

    void setUp() {
        auto password{yapet::toSecureArray("test")};
        aes256Factory = std::unique_ptr<yapet::Aes256Factory>{
            new yapet::Aes256Factory{password}};
    }

    void crypto() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*aes256Factory->crypto()) ==
                       typeid(yapet::Aes256));
#pragma clang diagnostic pop
    }

    void key() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*aes256Factory->key()) == typeid(yapet::Key256));
#pragma clang diagnostic pop

        yapet::Key256 key{};

        auto password{yapet::toSecureArray("test")};
        key.password(password);

        CPPUNIT_ASSERT(key == *aes256Factory->key());
    }

    void file() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*aes256Factory->file(
                           BUILDDIR "/aes256factory-test", true, false)) ==
                       typeid(yapet::Yapet20File));
#pragma clang diagnostic pop
    }

    void newFactory() {
        auto password{yapet::toSecureArray("test2")};
        auto factory{aes256Factory->newFactory(password)};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*factory) == typeid(yapet::Aes256Factory));
#pragma clang diagnostic pop
        yapet::Key256 key{};

        auto newPassword{yapet::toSecureArray("test2")};
        key.password(newPassword);

        CPPUNIT_ASSERT(key == *factory->key());
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Aes256FactoryTest::suite());
    return runner.run() ? 0 : 1;
}