#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "blowfishfactory.hh"
#include "key448.hh"
#include "testpaths.h"

class BlowfishFactoryTest : public CppUnit::TestFixture {
   private:
    std::unique_ptr<yapet::BlowfishFactory> blowfishFactory;

   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Blowfish Factory");

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishFactoryTest>(
            "should give correct crypto", &BlowfishFactoryTest::crypto));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishFactoryTest>(
            "should give correct key", &BlowfishFactoryTest::key));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishFactoryTest>(
            "should give correct file", &BlowfishFactoryTest::file));

        suiteOfTests->addTest(new CppUnit::TestCaller<BlowfishFactoryTest>(
            "should create proper new factory",
            &BlowfishFactoryTest::newFactory));

        return suiteOfTests;
    }

    void setUp() {
        auto password{yapet::toSecureArray("test")};
        blowfishFactory = std::unique_ptr<yapet::BlowfishFactory>{
            new yapet::BlowfishFactory{password}};
    }

    void crypto() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*blowfishFactory->crypto()) ==
                       typeid(yapet::Blowfish));
#pragma clang diagnostic pop
    }

    void key() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*blowfishFactory->key()) ==
                       typeid(yapet::Key448));
#pragma clang diagnostic pop
        yapet::Key448 key{};

        auto password{yapet::toSecureArray("test")};
        key.password(password);

        CPPUNIT_ASSERT(key == *blowfishFactory->key());
    }

    void file() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*blowfishFactory->file(
                           BUILDDIR "/blowfishfactory-test", true, false)) ==
                       typeid(yapet::Yapet10File));
#pragma clang diagnostic pop
    }

    void newFactory() {
        auto password{yapet::toSecureArray("test2")};
        auto factory{blowfishFactory->newFactory(password)};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
        CPPUNIT_ASSERT(typeid(*factory) == typeid(yapet::BlowfishFactory));
#pragma clang diagnostic pop
        yapet::Key448 key{};

        auto newPassword{yapet::toSecureArray("test2")};
        key.password(newPassword);

        CPPUNIT_ASSERT(key == *factory->key());
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(BlowfishFactoryTest::suite());
    return runner.run() ? 0 : 1;
}