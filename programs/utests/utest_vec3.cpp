#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/catch_get_random_seed.hpp>

#include <engine/eng_vec.h>

using namespace eng;

using FloatLimits = std::numeric_limits<float>;

namespace cg = Catch::Generators;

template<int Count>
std::array<float, Count> GenRandomFloats() {
	cg::RandomFloatingGenerator gen(-2000.0, 2000.0, Catch::getSeed());
	std::array<float, Count> nums;
	for (auto &num : nums) {
		gen.next();
		num = gen.get();
	}
	return nums;
}

TEST_CASE("Simple Addition")
{
	const auto [a1, b1, c1, a2, b2, c2] = GenRandomFloats<6>();

	const Vector3D<float> v1 {a1, b1, c1};
	const Vector3D<float> v2 {a2, b2, c2};

	const auto v3 = v1 + v2;
	static_assert(std::same_as<std::remove_cvref_t<decltype(v3)>, Vector3D<float>>);

	constexpr float eps = 1.0e-3; // a tolerance for how close the answer needs to be
	REQUIRE_THAT(v3.x(), Catch::Matchers::WithinAbs(a1 + a2, eps));
	REQUIRE_THAT(v3.y(), Catch::Matchers::WithinAbs(b1 + b2, eps));
	REQUIRE_THAT(v3.z(), Catch::Matchers::WithinAbs(c1 + c2, eps));
}

TEST_CASE("Simple Subtraction")
{
	const auto [a1, b1, c1, a2, b2, c2] = GenRandomFloats<6>();

	const Vector3D<float> v1 {a1, b1, c1};
	const Vector3D<float> v2 {a2, b2, c2};

	const auto v3 = v1 - v2;
	static_assert(std::same_as<std::remove_cvref_t<decltype(v3)>, Vector3D<float>>);

	constexpr float eps = 1.0e-3; // a tolerance for how close the answer needs to be
	REQUIRE_THAT(v3.x(), Catch::Matchers::WithinAbs(a1 - a2, eps));
	REQUIRE_THAT(v3.y(), Catch::Matchers::WithinAbs(b1 - b2, eps));
	REQUIRE_THAT(v3.z(), Catch::Matchers::WithinAbs(c1 - c2, eps));
}

TEST_CASE("Scalar Multiply")
{
	const auto [a, b, c, d] = GenRandomFloats<4>();

	const Vector3D<float> v1 {a, b, c};

	const auto v2 = v1 * d;
	static_assert(std::same_as<std::remove_cvref_t<decltype(v2)>, Vector3D<float>>);

	constexpr float eps = 1.0e-3; // a tolerance for how close the answer needs to be
	REQUIRE_THAT(v2.x(), Catch::Matchers::WithinAbs(a * d, eps));
	REQUIRE_THAT(v2.y(), Catch::Matchers::WithinAbs(b * d, eps));
	REQUIRE_THAT(v2.z(), Catch::Matchers::WithinAbs(c * d, eps));
}

TEST_CASE("Normalize")
{
	const Vector3D<float> v1 {4.325, 3.1415, 420.69};

	const auto v2 = v1.normalize();
	static_assert(std::same_as<std::remove_cvref_t<decltype(v2)>, Vector3D<float>>);

	REQUIRE_THAT(v2.x(), Catch::Matchers::WithinAbs(0.0102798994315, 1.0e-6));
	REQUIRE_THAT(v2.y(), Catch::Matchers::WithinAbs(0.00746689111304, 1.0e-7));
	REQUIRE_THAT(v2.z(), Catch::Matchers::WithinAbs(0.999919281346, 1.0e-5));
}

TEST_CASE("Clamp")
{
	const Vector3D<float> v1 {4.72, 5.0, 5.35234};

	const auto v2 = v1.clamp(5.0);
	static_assert(std::same_as<std::remove_cvref_t<decltype(v2)>, Vector3D<float>>);

	REQUIRE_THAT(v2.x(), Catch::Matchers::WithinAbs(4.72, 1.0e-3));
	REQUIRE_THAT(v2.y(), Catch::Matchers::WithinAbs(5.0, 1.0e-3));
	REQUIRE_THAT(v2.z(), Catch::Matchers::WithinAbs(5.0, 1.0e-3));
}
