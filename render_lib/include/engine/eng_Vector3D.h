#pragma once
#include <engine/eng_Rng.h>
#include <array>
#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>

#include <smmintrin.h>

#ifdef ENGINE_BUILD_GL_RENDER
#include "glm/glm.hpp"
#endif // ENGINE_BUILD_GL_RENDER

namespace eng {

template<typename ValueT = float, size_t N = 3>
class Vector {
public:
    using ValueType = std::remove_reference_t<ValueT>;

    constexpr Vector() noexcept {
        std::ranges::fill(this->e, static_cast<ValueType>(0));
    }

    template<typename... Args>
    requires (sizeof...(Args) == N)
    constexpr Vector(Args&&... args) : e{static_cast<ValueType>(args)...} {}

    template<typename OtherT, typename ValueT2, size_t N2>
    requires (N2 == (N - 1))
    constexpr Vector(const Vector<OtherT, N2>& other, ValueT2 extra) {
        std::ranges::copy(other.e, this->e.begin());
        this->e[N-1] = static_cast<ValueType>(extra);
    }

    template<typename OtherT>
    constexpr Vector(const Vector<OtherT, N>& other) {
        std::ranges::copy(other.e, this->e.begin());
    }

    template<std::ranges::range R>
    explicit constexpr Vector(R&& range) {
        std::ranges::copy(range, this->e.begin());
    }

    template<std::invocable G>
    explicit constexpr Vector(G&& generator) {
        std::ranges::generate(this->e, generator);
    }

    template<typename OtherT>
    requires std::is_arithmetic_v<OtherT>
    explicit constexpr Vector(OtherT value) {
        std::ranges::fill(this->e, static_cast<ValueT>(value));
    }

    [[nodiscard]] constexpr auto x() const noexcept                   { return e[0]; }
    [[nodiscard]] constexpr auto y() const noexcept requires (N >= 2) { return e[1]; }
    [[nodiscard]] constexpr auto z() const noexcept requires (N >= 3) { return e[2]; }
    [[nodiscard]] constexpr auto w() const noexcept requires (N >= 4) { return e[4]; }

    [[nodiscard]] constexpr auto r() const noexcept                   { return e[0]; }
    [[nodiscard]] constexpr auto g() const noexcept requires (N >= 2) { return e[1]; }
    [[nodiscard]] constexpr auto b() const noexcept requires (N >= 3) { return e[2]; }
    [[nodiscard]] constexpr auto a() const noexcept requires (N >= 4) { return e[4]; }

    [[nodiscard]] constexpr ValueType get_ptr() noexcept { return &e[0]; }

    [[nodiscard]] constexpr Vector<ValueT, N> operator-() const noexcept {
        return Vector(std::ranges::views::transform(e, [](ValueT val) { return -val; }));
    }

    template<typename Self>
    [[nodiscard]] constexpr auto&& operator[](this Self&& self, int index) {
        assert(index < N);
        return std::forward<Self>(self).e[index];
    }

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator+=(Vector<T, NE>& vec, const S& rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator+=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator-=(Vector<T, NE>& vec, S rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator-=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator*=(Vector<T, NE>& vec, S rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator*=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator/=(Vector<T, NE>& vec, S rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto& operator/=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator+(const Vector<T, NE> &lhs, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator+(const Vector<T, NE> &lhs, const S& rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator-(const Vector<T, NE> &lhs, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator-(const Vector<T, NE> &lhs, const S& rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator*(const Vector<T, NE> &lhs, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator*(const Vector<T, NE> &lhs, S rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator*(T lhs, const Vector<S, NE> &rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator/(const Vector<T, NE> &lhs, S rhs) noexcept;

    template<typename T, typename S, size_t NE>
    friend constexpr auto operator/(T lhs, const Vector<S, NE> &rhs) noexcept;

    template<typename T, size_t NE>
    friend constexpr double dot(const Vector<T, NE> &u, const Vector<T, NE> &v) noexcept;

    [[nodiscard]] constexpr ValueType length() const noexcept { // cxx26 constexpr
        return std::sqrt(this->length_squared());
    }

    [[nodiscard]] constexpr ValueType length_squared() const noexcept {
        return std::ranges::fold_left(this->e, 0, [](ValueType v1, ValueType v2) { return v1 + v2 * v2; });
    }

    [[nodiscard]] constexpr Vector<ValueT, N> normalize() const noexcept {
        const auto length = this->length();

        /* Zero special case. */
        if (length == 0.0)
            return {0,0,0};

        return Vector(this->e | std::ranges::views::transform([length](ValueType val) { return val / length; }));
    }

    [[nodiscard]] constexpr Vector<ValueT, N> clamp(ValueType max) const noexcept {
        return Vector(this->e | std::ranges::views::transform([max](ValueType val) { return std::min(val, max); }));
    }

    [[nodiscard]] static Vector<ValueT, N> random(ValueType min, ValueType max) {
        Rng<ValueType> gen(min, max);
        return Vector(gen);
    }

    [[nodiscard]] static Vector<ValueT, N> random_in_unit_sphere() {
        Vector<ValueT, N> vec;
        while (vec = random(-1.0, 1.0), vec.length() > 1) {}
        return vec;
    }
#ifdef ENGINE_BUILD_GL_RENDER
    // TODO: template me
    [[nodiscard]] glm::vec3 ToGlmVector() const noexcept {
        return glm::vec3(e[0], e[1], e[2]);
    } 

    // TODO: and me
    [[nodiscard]] static Vector<ValueT, N> FromGlmVector(const glm::vec3& glmVec) noexcept {
        return Vector<ValueT, N>(glmVec.x, glmVec.y, glmVec.z);
    }
#endif // ENGINE_BUILD_GL_RENDER

    // default constructor sets everything to zero (in theory...)
    static constexpr Vector<ValueT, N> Zero() { return Vector(); }
private:
    union {
        std::array<ValueT, N> e;
        __m128 v;
    };
}; // class Vector

template<typename T>
using Vector3D = Vector<T, 3>;

using Vector3DF = Vector<float, 3>;
static_assert(std::is_standard_layout_v<Vector3DF>);

template<typename T, typename S, size_t NE>
constexpr auto& operator+=(Vector<T, NE>& vec, const S& rhs) noexcept {
    auto f = static_cast<Vector<T>::ValueType>(rhs);
    for (auto& e : vec.e)
        e += f;
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator+=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept {
    for (size_t i = 0; i < NE; i++)
        vec.e[i] += rhs.e[i];
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator-=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept {
    for (size_t i = 0; i < NE; i++)
        vec.e[i] -= rhs.e[i];
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator-=(Vector<T, NE>& vec, S rhs) noexcept {
    auto f = static_cast<Vector<T>::ValueType>(rhs);
    for (auto& e : vec)
        e -= f;
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator*=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept {
    for (size_t i = 0; i < NE; i++)
        vec.e[i] *= rhs.e[i];
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator*=(Vector<T, NE>& vec, S rhs) noexcept {
    auto f = static_cast<Vector<T>::ValueType>(rhs);
    for (auto& e : vec)
        e *= f;
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator/=(Vector<T, NE>& vec, const Vector<S, NE> &rhs) noexcept {
    for (size_t i = 0; i < NE; i++)
        vec.e[i] /= rhs.e[i];
    return vec;
}

template<typename T, typename S, size_t NE>
constexpr auto& operator/=(Vector<T, NE>& vec, S rhs) noexcept {
    auto f = static_cast<Vector<T, NE>::ValueType>(rhs);
    for (auto& e : vec)
        e /= f;
    return vec;
}

template<typename ValueT, size_t NE>
inline std::ostream& operator<<(std::ostream &out, const Vector<ValueT, NE> &vec) {
    return out << vec.x() << ' ' << vec.y() << ' ' << vec.z();
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator+(const Vector<ValueT, NE> &lhs, const Vector<ValueS, NE> &rhs) noexcept {
    size_t i = 0;
    return Vector([&]() { return lhs.e[i] + rhs.e[i++]; });
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator+(const Vector<ValueT, NE> &lhs, const ValueS& rhs) noexcept {
    return Vector(lhs.e | std::ranges::views::transform([rhs](auto val) { return val + rhs; }));
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator-(const Vector<ValueT, NE> &lhs, const Vector<ValueS, NE> &rhs) noexcept {
    size_t i = 0;
    return Vector([&]() { return lhs.e[i] - rhs.e[i++]; });
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator-(const Vector<ValueT, NE> &lhs, const ValueS& rhs) noexcept {
    return Vector(lhs.e | std::ranges::views::transform([rhs](auto val) { return val - rhs; }));
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator*(const Vector<ValueT, NE> &lhs, const Vector<ValueS, NE> &rhs) noexcept {
    size_t i = 0;
    return Vector([&]() { return lhs.e[i] * rhs.e[i++]; });
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator*(const Vector<ValueT, NE> &lhs, ValueS rhs) noexcept {
    return Vector(lhs.e | std::ranges::views::transform([rhs](auto val) { return val * rhs; }));
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator*(ValueT lhs, const Vector<ValueS, NE> &rhs) noexcept {
    return rhs * lhs;
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator/(const Vector<ValueT, NE> &lhs, ValueS rhs) noexcept {
    return Vector(lhs.e | std::ranges::views::transform([rhs](auto val) { return val / rhs; }));
}

template<typename ValueT, typename ValueS, size_t NE>
[[nodiscard]] constexpr auto operator/(ValueT lhs, const Vector<ValueS, NE> &rhs) noexcept {
    return Vector(rhs.e | std::ranges::views::transform([lhs](auto val) { return lhs / val; }));
}

template<typename ValueT, typename WithinT, size_t NE>
[[nodiscard]] constexpr bool equal_within(const Vector<ValueT, NE>& lhs, const Vector<ValueT, NE>& rhs, const WithinT& within) noexcept {
    const auto withint = static_cast<std::remove_reference_t<ValueT>>(within);
    return std::ranges::equal(lhs, rhs, [withint](auto v1, auto v2) {
        return v1 > v2 + withint || v1 < v2 + withint;
    });
}

// DO NOT use this for comparing math results
template<typename ValueT, size_t NE>
[[nodiscard]] constexpr bool equal_exact(const Vector<ValueT, NE>& lhs, const Vector<ValueT, NE>& rhs) noexcept {
    return std::ranges::equal(lhs, rhs);
}

template<typename ValueT, size_t NE>
[[nodiscard]] constexpr double dot(const Vector<ValueT, NE> &u, const Vector<ValueT, NE> &v) noexcept {
    if constexpr (NE < 5) {
        if !consteval {
            return std::bit_cast<float>(_mm_extract_ps(_mm_dp_ps(u.v, v.v, 0x71), 0));
        }
    }

    decltype(u[0]+v[0]) sum = 0;
    for (size_t i = 0; i < 3; i++)
        sum += u[i] * v[i];

    return sum;
}

// TODO: Lookup what cross product does, later
template<typename ValueT, size_t NE>
[[nodiscard]] constexpr Vector<ValueT, NE> cross(const Vector<ValueT, NE> &u, const Vector<ValueT, NE> &v) noexcept {
    return Vector { u.y() * v.z() - u.z() * v.y(),
                  u.z() * v.x() - u.x() * v.z(),
                  u.x() * v.y() - u.y() * v.x() };
}

} // namespace eng

