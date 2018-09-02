#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include "passwordlistitem.hh"

constexpr auto NAME_CHAR{"name"};
// Length includes '\0'
constexpr auto NAME_LEN{5};

constexpr auto ENCRYPTED{"encrypted"};
// Length includes '\0'
constexpr auto ENCRYPTED_LEN{10};

class PasswordListItemTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Password List Item");

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "should properly get fields", &PasswordListItemTest::getter));

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "Copy ctor and assignment", &PasswordListItemTest::copyCtor));
        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "Move ctor and assignment", &PasswordListItemTest::moveCtor));

        return suiteOfTests;
    }

    void getter() {
        auto encrypted{yapet::toSecureArray(ENCRYPTED)};

        yapet::PasswordListItem passwordListItem{NAME_CHAR, encrypted};

        CPPUNIT_ASSERT(
            std::memcmp(passwordListItem.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(encrypted == passwordListItem.encryptedRecord());
    }

    void copyCtor() {
        auto encrypted{yapet::toSecureArray(ENCRYPTED)};

        yapet::PasswordListItem passwordListItem{NAME_CHAR, encrypted};

        yapet::PasswordListItem copied{passwordListItem};

        CPPUNIT_ASSERT(
            std::memcmp(passwordListItem.name(), copied.name(), NAME_LEN) == 0);
        CPPUNIT_ASSERT(passwordListItem.name() != copied.name());

        CPPUNIT_ASSERT(passwordListItem.encryptedRecord() ==
                       copied.encryptedRecord());

        auto copied2 = passwordListItem;
        CPPUNIT_ASSERT(std::memcmp(passwordListItem.name(), copied2.name(),
                                   NAME_LEN) == 0);
        CPPUNIT_ASSERT(passwordListItem.name() != copied2.name());

        CPPUNIT_ASSERT(passwordListItem.encryptedRecord() ==
                       copied2.encryptedRecord());
    }

    void moveCtor() {
        auto encrypted{yapet::toSecureArray(ENCRYPTED)};

        yapet::PasswordListItem passwordListItem{NAME_CHAR, encrypted};

        yapet::PasswordListItem moved{std::move(passwordListItem)};

        CPPUNIT_ASSERT(std::memcmp(moved.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(passwordListItem.name() != moved.name());

        CPPUNIT_ASSERT(passwordListItem.encryptedRecord() !=
                       moved.encryptedRecord());
        CPPUNIT_ASSERT(moved.encryptedRecord() == encrypted);

        passwordListItem = yapet::PasswordListItem{NAME_CHAR, encrypted};

        auto moved2 = std::move(passwordListItem);
        CPPUNIT_ASSERT(std::memcmp(moved2.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(passwordListItem.name() != moved2.name());

        CPPUNIT_ASSERT(passwordListItem.encryptedRecord() !=
                       moved2.encryptedRecord());
        CPPUNIT_ASSERT(moved2.encryptedRecord() == encrypted);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(PasswordListItemTest::suite());
    return runner.run() ? 0 : 1;
}
