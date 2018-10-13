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

constexpr auto NAME_CHAR_2{"name 2"};

constexpr auto ENCRYPTED{"encrypted"};

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

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "Default constructor", &PasswordListItemTest::defaultConstructor));

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "Should properly use comperators",
            &PasswordListItemTest::comperators));

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordListItemTest>(
            "Should properly cast to std::string",
            &PasswordListItemTest::stringCastOperator));

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

    void defaultConstructor() {
        yapet::PasswordListItem passwordListItem;
        CPPUNIT_ASSERT(passwordListItem.name() == nullptr);
        CPPUNIT_ASSERT(passwordListItem.nameSize() == 0);
    }

    void comperators() {
        auto encrypted{yapet::toSecureArray(ENCRYPTED)};
        yapet::PasswordListItem passwordListItem1{NAME_CHAR, encrypted};
        yapet::PasswordListItem passwordListItem2{NAME_CHAR, encrypted};
        yapet::PasswordListItem passwordListItem3{NAME_CHAR_2, encrypted};

        CPPUNIT_ASSERT(passwordListItem1 == passwordListItem1);
        CPPUNIT_ASSERT(passwordListItem1 == passwordListItem2);

        CPPUNIT_ASSERT(!(passwordListItem1 != passwordListItem1));
        CPPUNIT_ASSERT(!(passwordListItem1 != passwordListItem2));

        CPPUNIT_ASSERT(passwordListItem1 != passwordListItem3);
        CPPUNIT_ASSERT(!(passwordListItem1 == passwordListItem3));

        CPPUNIT_ASSERT(!(passwordListItem1 < passwordListItem2));
        CPPUNIT_ASSERT(passwordListItem1 < passwordListItem3);
        CPPUNIT_ASSERT(!(passwordListItem3 < passwordListItem1));

        CPPUNIT_ASSERT(!(passwordListItem1 > passwordListItem2));
        CPPUNIT_ASSERT(!(passwordListItem1 > passwordListItem3));
        CPPUNIT_ASSERT(passwordListItem3 > passwordListItem1);
    }

    void stringCastOperator() {
        auto encrypted{yapet::toSecureArray(ENCRYPTED)};
        yapet::PasswordListItem passwordListItem1{NAME_CHAR, encrypted};

        std::string actual{passwordListItem1};
        std::string expected{NAME_CHAR};
        CPPUNIT_ASSERT(actual == expected);
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(PasswordListItemTest::suite());
    return runner.run() ? 0 : 1;
}
