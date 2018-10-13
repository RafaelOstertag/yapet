#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cstring>

#include "passwordrecord.hh"

constexpr auto NAME_CHAR{"name"};
// Length includes '\0'
constexpr auto NAME_LEN{5};

constexpr auto HOST_CHAR{"host"};
// Length includes '\0'
constexpr auto HOST_LEN{5};

constexpr auto USERNAME_CHAR{"username"};
// Length includes '\0'
constexpr auto USERNAME_LEN{9};

constexpr auto PASSWORD_CHAR{"password"};
// Length includes '\0'
constexpr auto PASSWORD_LEN{9};

constexpr auto COMMENT_CHAR{"comment"};
// Length includes '\0'
constexpr auto COMMENT_LEN{8};

constexpr std::uint8_t serializedPasswordRecord[] = {
    'n', 'a', 'm', 'e',  '\0', 'h', 'o',  's', 't', '\0', 'u', 's',
    'e', 'r', 'n', 'a',  'm',  'e', '\0', 'p', 'a', 's',  's', 'w',
    'o', 'r', 'd', '\0', 'c',  'o', 'm',  'm', 'e', 'n',  't', '\0'};

inline yapet::PasswordRecord makeTestPasswordRecordFromChar() {
    yapet::PasswordRecord passwordRecord{};

    passwordRecord.name(NAME_CHAR);
    passwordRecord.host(HOST_CHAR);
    passwordRecord.username(USERNAME_CHAR);
    passwordRecord.password(PASSWORD_CHAR);
    passwordRecord.comment(COMMENT_CHAR);

    return passwordRecord;
}

class PasswordRecordTest : public CppUnit::TestFixture {
   public:
    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
            new CppUnit::TestSuite("Password Record");

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "Char Setter and getter",
            &PasswordRecordTest::charSetterAndGetter));
        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "uint8_t  Setter and getter",
            &PasswordRecordTest::uint8SetterAndGetter));
        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "serialize and deserialize",
            &PasswordRecordTest::serializeAndDeserialize));

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "deserialize non-matching size",
            &PasswordRecordTest::deserializeNonMatchingSize));

        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "Copy ctor and assignment", &PasswordRecordTest::copyCtor));
        suiteOfTests->addTest(new CppUnit::TestCaller<PasswordRecordTest>(
            "Move ctor and assignment", &PasswordRecordTest::moveCtor));

        return suiteOfTests;
    }

    void charSetterAndGetter() {
        yapet::PasswordRecord passwordRecord{makeTestPasswordRecordFromChar()};

        CPPUNIT_ASSERT(
            std::memcmp(passwordRecord.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(passwordRecord.host(), HOST_CHAR, HOST_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.username(), USERNAME_CHAR,
                                   USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.password(), PASSWORD_CHAR,
                                   PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.comment(), COMMENT_CHAR,
                                   COMMENT_LEN) == 0);
    }

    void uint8SetterAndGetter() {
        yapet::PasswordRecord passwordRecord{};

        passwordRecord.name(reinterpret_cast<const std::uint8_t *>(NAME_CHAR),
                            NAME_LEN);
        passwordRecord.host(reinterpret_cast<const std::uint8_t *>(HOST_CHAR),
                            HOST_LEN);
        passwordRecord.username(
            reinterpret_cast<const std::uint8_t *>(USERNAME_CHAR),
            USERNAME_LEN);
        passwordRecord.password(
            reinterpret_cast<const std::uint8_t *>(PASSWORD_CHAR),
            PASSWORD_LEN);
        passwordRecord.comment(
            reinterpret_cast<const std::uint8_t *>(COMMENT_CHAR), COMMENT_LEN);

        CPPUNIT_ASSERT(
            std::memcmp(passwordRecord.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(passwordRecord.host(), HOST_CHAR, HOST_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.username(), USERNAME_CHAR,
                                   USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.password(), PASSWORD_CHAR,
                                   PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(passwordRecord.comment(), COMMENT_CHAR,
                                   COMMENT_LEN) == 0);
    }

    void serializeAndDeserialize() {
        yapet::PasswordRecord passwordRecord{};

        passwordRecord.name(NAME_CHAR);
        passwordRecord.host(HOST_CHAR);
        passwordRecord.username(USERNAME_CHAR);
        passwordRecord.password(PASSWORD_CHAR);
        passwordRecord.comment(COMMENT_CHAR);

        auto serialized = passwordRecord.serialize();
        CPPUNIT_ASSERT(serialized.size() == yapet::PasswordRecord::TOTAL_SIZE);
        CPPUNIT_ASSERT(std::memcmp(
            *serialized, serializedPasswordRecord,
            NAME_LEN + HOST_LEN + USERNAME_LEN + PASSWORD_LEN + COMMENT_LEN));

        yapet::PasswordRecord fromSerialized{serialized};

        CPPUNIT_ASSERT(
            std::memcmp(fromSerialized.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(fromSerialized.host(), HOST_CHAR, HOST_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(fromSerialized.username(), USERNAME_CHAR,
                                   USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(fromSerialized.password(), PASSWORD_CHAR,
                                   PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(fromSerialized.comment(), COMMENT_CHAR,
                                   COMMENT_LEN) == 0);
    }

    void deserializeNonMatchingSize() {
        yapet::SecureArray tooSmall{yapet::PasswordRecord::TOTAL_SIZE - 1};
        CPPUNIT_ASSERT_THROW(yapet::PasswordRecord{tooSmall},
                             yapet::DeserializationError);

        yapet::SecureArray tooBig{yapet::PasswordRecord::TOTAL_SIZE + 1};
        CPPUNIT_ASSERT_THROW(yapet::PasswordRecord{tooSmall},
                             yapet::DeserializationError);
    }

    void copyCtor() {
        yapet::PasswordRecord passwordRecord{makeTestPasswordRecordFromChar()};

        yapet::PasswordRecord copied{passwordRecord};

        CPPUNIT_ASSERT(std::memcmp(copied.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(copied.host(), HOST_CHAR, HOST_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(copied.username(), USERNAME_CHAR, USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(copied.password(), PASSWORD_CHAR, PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(copied.comment(), COMMENT_CHAR, COMMENT_LEN) == 0);

        CPPUNIT_ASSERT(passwordRecord.name() != copied.name());
        CPPUNIT_ASSERT(passwordRecord.host() != copied.host());
        CPPUNIT_ASSERT(passwordRecord.username() != copied.username());
        CPPUNIT_ASSERT(passwordRecord.password() != copied.password());
        CPPUNIT_ASSERT(passwordRecord.comment() != copied.comment());

        yapet::PasswordRecord copyAssigned = passwordRecord;
        CPPUNIT_ASSERT(std::memcmp(copyAssigned.name(), NAME_CHAR, NAME_LEN) ==
                       0);
        CPPUNIT_ASSERT(std::memcmp(copyAssigned.host(), HOST_CHAR, HOST_LEN) ==
                       0);
        CPPUNIT_ASSERT(std::memcmp(copyAssigned.username(), USERNAME_CHAR,
                                   USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(copyAssigned.password(), PASSWORD_CHAR,
                                   PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(copyAssigned.comment(), COMMENT_CHAR,
                                   COMMENT_LEN) == 0);

        CPPUNIT_ASSERT(passwordRecord.name() != copyAssigned.name());
        CPPUNIT_ASSERT(passwordRecord.host() != copyAssigned.host());
        CPPUNIT_ASSERT(passwordRecord.username() != copyAssigned.username());
        CPPUNIT_ASSERT(passwordRecord.password() != copyAssigned.password());
        CPPUNIT_ASSERT(passwordRecord.comment() != copyAssigned.comment());
    }

    void moveCtor() {
        yapet::PasswordRecord passwordRecord{makeTestPasswordRecordFromChar()};

        yapet::PasswordRecord moved{std::move(passwordRecord)};

        CPPUNIT_ASSERT(std::memcmp(moved.name(), NAME_CHAR, NAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(moved.host(), HOST_CHAR, HOST_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(moved.username(), USERNAME_CHAR, USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(moved.password(), PASSWORD_CHAR, PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(
            std::memcmp(moved.comment(), COMMENT_CHAR, COMMENT_LEN) == 0);

        CPPUNIT_ASSERT(passwordRecord.name() != moved.name());
        CPPUNIT_ASSERT(passwordRecord.host() != moved.host());
        CPPUNIT_ASSERT(passwordRecord.username() != moved.username());
        CPPUNIT_ASSERT(passwordRecord.password() != moved.password());
        CPPUNIT_ASSERT(passwordRecord.comment() != moved.comment());

        passwordRecord = makeTestPasswordRecordFromChar();
        yapet::PasswordRecord moveAssigned = std::move(passwordRecord);

        CPPUNIT_ASSERT(std::memcmp(moveAssigned.name(), NAME_CHAR, NAME_LEN) ==
                       0);
        CPPUNIT_ASSERT(std::memcmp(moveAssigned.host(), HOST_CHAR, HOST_LEN) ==
                       0);
        CPPUNIT_ASSERT(std::memcmp(moveAssigned.username(), USERNAME_CHAR,
                                   USERNAME_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(moveAssigned.password(), PASSWORD_CHAR,
                                   PASSWORD_LEN) == 0);
        CPPUNIT_ASSERT(std::memcmp(moveAssigned.comment(), COMMENT_CHAR,
                                   COMMENT_LEN) == 0);

        CPPUNIT_ASSERT(passwordRecord.name() != moveAssigned.name());
        CPPUNIT_ASSERT(passwordRecord.host() != moveAssigned.host());
        CPPUNIT_ASSERT(passwordRecord.username() != moveAssigned.username());
        CPPUNIT_ASSERT(passwordRecord.password() != moveAssigned.password());
        CPPUNIT_ASSERT(passwordRecord.comment() != moveAssigned.comment());
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(PasswordRecordTest::suite());
    return runner.run() ? 0 : 1;
}
