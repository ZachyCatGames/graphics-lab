#include "ObjectManager.h"
#include "ObjectBase.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <functional>

class TestObject : public eng::ObjectBase<TestObject> {
public:
    using Func = std::function<void()>;
    TestObject(Func initf, Func endf) : m_endf(endf) {
        initf();
    }

    ~TestObject() {
        m_endf();
    }

private:
    Func m_endf;
}; // class TestObject

using Manager = eng::ObjectManager<TestObject>;

TEST_CASE( "Allocate Free Single" )
{
    /* We should have zero objects at the start. */
    auto p_manager = Manager::Get();
    REQUIRE(p_manager->GetActiveObjectCount() == 0);

    int num = 0;
    {
        /* Allocate an object. */
        auto obj = TestObject::Create([&]() {num = 1; }, [&]() { num = 2; });

        /* Check number. */
        REQUIRE(num == 1);
        REQUIRE(p_manager->GetActiveObjectCount() == 1);
    }

    REQUIRE(num == 2);
    REQUIRE(p_manager->GetActiveObjectCount() == 0);
}

TEST_CASE( "Allocate Free 200" )
{
    /* We should have zero objects at the start. */
    auto p_manager = Manager::Get();
    REQUIRE(p_manager->GetActiveObjectCount() == 0);

    int num = 0;
    {
        /* Allocate an object. */
        auto obj = TestObject::Create([&]() {num = 1; }, [&]() { num = 2; });

        /* Check number. */
        REQUIRE(num == 1);
        REQUIRE(p_manager->GetActiveObjectCount() == 1);
    }

    REQUIRE(num == 2);
    REQUIRE(p_manager->GetActiveObjectCount() == 0);
}

TEST_CASE( "Simple Success Test" )
{
    int i = 42;
    REQUIRE( i == 42 );

    float myCalcOfPi = 3.141f;

    float eps = 1.0e-3; // a tolerance for how close the answer needs to be
    float actualAnswer = 3.14159f;
    REQUIRE_THAT( myCalcOfPi, Catch::Matchers::WithinAbs(actualAnswer, eps));
}
