#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "securearray.hh"

class SecureArrayTest : public CppUnit::TestFixture {
public:

    static CppUnit::TestSuite *suite() {
        CppUnit::TestSuite *suiteOfTests =
                new CppUnit::TestSuite("SecureArrayTest");
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "should not initialize with negative size",
                &SecureArrayTest::testInitWithZeroSize));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "should set empty array upon destruction",
                &SecureArrayTest::testZeroOutBuffer));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "copy constructor", &SecureArrayTest::testCopy));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "assignment", &SecureArrayTest::testAssignment));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "move", &SecureArrayTest::testMove));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "move assignment", &SecureArrayTest::testMove));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "equality", &SecureArrayTest::testEquality));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "equality", &SecureArrayTest::testEquality));
        suiteOfTests->addTest(new CppUnit::TestCaller<SecureArrayTest>(
                "equality", &SecureArrayTest::testIndexOperator));

        return suiteOfTests;
    }

    void testInitWithZeroSize() {
        CPPUNIT_ASSERT_THROW(yapet::SecureArray secureArray{0},
        std::invalid_argument);
    }

    void testZeroOutBuffer() {
        auto subject = new yapet::SecureArray{3};

        for (auto i = 0; i < 3; i++) {
            subject->operator*()[i] = 42;
        }

        std::uint8_t *array = **subject;
        delete subject;
        for (auto i = 0; i < 3; i++) {
            CPPUNIT_ASSERT_EQUAL((std::uint8_t)0, array[i]);
        }
    }

    void testCopy() {
        yapet::SecureArray a = yapet::SecureArray(1);
        **a = 42;

        yapet::SecureArray b = yapet::SecureArray{a};

        CPPUNIT_ASSERT_EQUAL((std::uint8_t)42, **a);
        CPPUNIT_ASSERT_EQUAL(**a, **b);

        CPPUNIT_ASSERT(*a != *b);
    }

    void testAssignment() {
        yapet::SecureArray a = yapet::SecureArray{1};
        **a = 43;

        yapet::SecureArray b = yapet::SecureArray{1};
        **b = 42;

        a = b;

        CPPUNIT_ASSERT_EQUAL((std::uint8_t)42, **a);
        CPPUNIT_ASSERT(*a != *b);
    }

    void testMove() {
        yapet::SecureArray a = yapet::SecureArray{1};
        **a = 42;

        yapet::SecureArray b{std::move(a)};

        CPPUNIT_ASSERT_EQUAL((std::uint8_t *)nullptr, *a);
        CPPUNIT_ASSERT_EQUAL((std::uint8_t)42, **b);
    }

    void testMoveAssignment() {
        yapet::SecureArray a = yapet::SecureArray{1};
        **a = 42;
        std::uint8_t *ptrAArray = *a;

        yapet::SecureArray b = yapet::SecureArray{1};
        **b = 43;
        std::uint8_t *ptrBArray = *b;

        b = std::move(a);

        CPPUNIT_ASSERT_EQUAL((std::uint8_t *)nullptr, *a);
        CPPUNIT_ASSERT_EQUAL((std::uint8_t)42, **b);
        CPPUNIT_ASSERT_EQUAL(ptrAArray, ptrBArray);
        CPPUNIT_ASSERT_EQUAL((std::uint8_t)0, *ptrAArray);
    }

    void testEquality() {
        yapet::SecureArray a = yapet::SecureArray{1};
        **a = 42;

        yapet::SecureArray b = yapet::SecureArray{1};
        **b = 42;

        CPPUNIT_ASSERT(a == b);
        CPPUNIT_ASSERT(!(a != b));

        b = yapet::SecureArray{1};
        **b = 43;

        CPPUNIT_ASSERT(!(a == b));
        CPPUNIT_ASSERT(a != b);
    }

    void testIndexOperator() {
        yapet::SecureArray secureArray(3);

        CPPUNIT_ASSERT_THROW(secureArray[-1], std::out_of_range);
        CPPUNIT_ASSERT_THROW(secureArray[4], std::out_of_range);

        for (auto i = 0; i < 3; i++) {
            (*secureArray)[i] = i;
            CPPUNIT_ASSERT_EQUAL((std::uint8_t)i, secureArray[i]);
        }
    }
};

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(SecureArrayTest::suite());
    return runner.run() ? 0 : 1;
}
