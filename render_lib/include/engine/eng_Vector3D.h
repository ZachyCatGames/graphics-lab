#pragma once
#include <engine/eng_Rng.h>
#include <cmath>
#include <iostream>

namespace eng {

template<typename ValueT = float>
class Vector3D {
public:
    using ValueType = std::remove_reference_t<ValueT>;

    constexpr Vector3D() noexcept : e{0,0,0} {}
    constexpr Vector3D(ValueT x, ValueT y, ValueT z) : e{x,y,z} {}

    template<typename OtherT>
    constexpr Vector3D(const Vector3D<OtherT>& other) :
        e{static_cast<ValueType>(other.x()), static_cast<ValueType>(other.y()), static_cast<ValueType>(other.z())}
        {}

    [[nodiscard]] constexpr auto x() const noexcept { return e[0]; }
    [[nodiscard]] constexpr auto y() const noexcept { return e[1]; }
    [[nodiscard]] constexpr auto z() const noexcept { return e[2]; }

    [[nodiscard]] constexpr Vector3D operator-() const noexcept { return Vector3D{-this->x(), -this->y(), -this->z()}; }

    template<typename Self>
    [[nodiscard]] constexpr auto&& operator[](this Self&& self, int index) {
        return std::forward<Self>(self).e[index];
    }

    template<typename T, typename S>
    friend constexpr auto& operator+=(Vector3D<T>& vec, const S& rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator+=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator-=(Vector3D<T>& vec, S rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator-=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator*=(Vector3D<T>& vec, S rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator*=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator/=(Vector3D<T>& vec, S rhs) noexcept;

    template<typename T, typename S>
    friend constexpr auto& operator/=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept;

    [[nodiscard]] constexpr ValueType length() const noexcept { // cxx26 constexpr
        return std::sqrt(this->length_squared());
    }

    [[nodiscard]] constexpr ValueType length_squared() const noexcept {
        return e[0]*e[0] + e[1] * e[1] + e[2] * e[2];
    }

    [[nodiscard]] constexpr Vector3D<ValueT> normalize() const noexcept {
        const auto length = this->length();

        /* Zero special case. */
        if (length == 0.0)
            return {0,0,0};

        return {this->x() / length, this->y() / length, this->z() / length};
    }

    [[nodiscard]] constexpr Vector3D<ValueT> clamp(ValueType max) const noexcept {
        const auto x = std::min(this->x(), max);
        const auto y = std::min(this->y(), max);
        const auto z = std::min(this->z(), max);
        return {x, y, z};
    }

    [[nodiscard]] static Vector3D<ValueT> random(ValueType min, ValueType max) {
        Rng<ValueType> gen(min, max);
        return {gen(), gen(), gen()};
    }

    [[nodiscard]] static Vector3D<ValueT> random_in_unit_sphere() {
        Vector3D<ValueT> vec;
        while (vec = random(-1.0, 1.0), vec.length() > 1) {}
        return vec;
    }
private:
    ValueT e[3];
}; // class Vector3D

using Vector3DF = Vector3D<float>;

template<typename T, typename S>
constexpr auto& operator+=(Vector3D<T>& vec, const S& rhs) noexcept {
    auto f = static_cast<Vector3D<T>::ValueType>(rhs);
    vec.e[0] += f;
    vec.e[1] += f;
    vec.e[2] += f;
    return vec;
}

template<typename T, typename S>
constexpr auto& operator+=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept {
    vec.e[0] += rhs.e[0];
    vec.e[1] += rhs.e[1];
    vec.e[2] += rhs.e[2];
    return vec;
}

template<typename T, typename S>
constexpr auto& operator-=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept {
    vec.e[0] -= rhs.x();
    vec.e[1] -= rhs.y();
    vec.e[2] -= rhs.z();
    return vec;
}

template<typename T, typename S>
constexpr auto& operator-=(Vector3D<T>& vec, S rhs) noexcept {
    auto f = static_cast<Vector3D<T>::ValueType>(rhs);
    vec.e[0] -= f;
    vec.e[1] -= f;
    vec.e[2] -= f;
    return vec;
}

template<typename T, typename S>
constexpr auto& operator*=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept {
    vec.e[0] *= rhs.x();
    vec.e[1] *= rhs.y();
    vec.e[2] *= rhs.z();
    return vec;
}

template<typename T, typename S>
constexpr auto& operator*=(Vector3D<T>& vec, S rhs) noexcept {
    auto f = static_cast<Vector3D<T>::ValueType>(rhs);
    vec.e[0] *= f;
    vec.e[1] *= f;
    vec.e[2] *= f;
    return vec;
}

template<typename T, typename S>
constexpr auto& operator/=(Vector3D<T>& vec, const Vector3D<S> &rhs) noexcept {
    vec.e[0] /= rhs.x();
    vec.e[1] /= rhs.y();
    vec.e[2] /= rhs.z();
    return vec;
}

template<typename T, typename S>
constexpr auto& operator/=(Vector3D<T>& vec, S rhs) noexcept {
    auto f = static_cast<Vector3D<T>::ValueType>(rhs);
    vec.e[0] /= f;
    vec.e[1] /= f;
    vec.e[2] /= f;
    return vec;
}

template<typename ValueT>
inline std::ostream& operator<<(std::ostream &out, const Vector3D<ValueT> &vec) {
    return out << vec.x() << ' ' << vec.y() << ' ' << vec.z();
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator+(const Vector3D<ValueT> &lhs, const Vector3D<ValueS> &rhs) noexcept {
    return Vector3D{lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z()};
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator+(const Vector3D<ValueT> &lhs, const ValueS& rhs) noexcept {
    return Vector3D{lhs.x() + rhs, lhs.y() + rhs, lhs.z() + rhs};
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator-(const Vector3D<ValueT> &lhs, const Vector3D<ValueS> &rhs) noexcept {
    return Vector3D{lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z()};
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator-(const Vector3D<ValueT> &lhs, const ValueS& rhs) noexcept {
    return Vector3D{lhs.x() - rhs, lhs.y() - rhs, lhs.z() - rhs};
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator*(const Vector3D<ValueT> &lhs, const Vector3D<ValueS> &rhs) noexcept {
    return Vector3D{lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z()};
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator*(const Vector3D<ValueT> &lhs, ValueS rhs) noexcept {
    return Vector3D { lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs };
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator*(ValueT lhs, const Vector3D<ValueS> &rhs) noexcept {
    return rhs * lhs;
}

template<typename ValueT, typename ValueS>
[[nodiscard]] constexpr auto operator/(const Vector3D<ValueT> &lhs, ValueS rhs) noexcept {
    return (1 / rhs) * lhs;
}

template<typename ValueT, typename WithinT>
[[nodiscard]] constexpr bool equal_within(const Vector3D<ValueT>& lhs, const Vector3D<ValueT>& rhs, const WithinT& within) noexcept {
    const auto withint = static_cast<std::remove_reference_t<ValueT>>(within);
    if (lhs.x() > rhs.x() + withint || lhs.x() < rhs.x() - withint)
        return false;
    if (lhs.y() > rhs.y() + withint || lhs.y() < rhs.y() - withint)
        return false;
    if (lhs.z() > rhs.z() + withint || lhs.z() < rhs.z() - withint)
        return false;
    return true; 
}

// DO NOT use this for comparing math results
template<typename ValueT>
[[nodiscard]] constexpr bool equal_exact(const Vector3D<ValueT>& lhs, const Vector3D<ValueT>& rhs) noexcept {
    return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

template<typename ValueT>
[[nodiscard]] constexpr double dot(const Vector3D<ValueT> &u, const Vector3D<ValueT> &v) noexcept {
    return u.x() * v.x() +
           u.y() * v.y() +
           u.z() * v.z();
}

template<typename ValueT>
[[nodiscard]] constexpr Vector3D<ValueT> cross(const Vector3D<ValueT> &u, const Vector3D<ValueT> &v) noexcept {
    return Vector3D { u.y() * v.z() - u.z() * v.y(),
                  u.z() * v.x() - u.x() * v.z(),
                  u.x() * v.y() - u.y() * v.x() };
}

} // namespace eng

