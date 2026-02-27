#pragma once
#include <ctime>
#include <type_traits>
#include <random>

namespace eng {

template<typename T>
requires std::is_arithmetic_v<T>
class Rng {
public:
    using value_type = std::remove_reference_t<T>;

    Rng(value_type min, value_type max) : m_dist(min, max) {}

    void Reset() { m_dist.reset(); }

    auto GetNext() {
        //if (!s_init)
        //    s_rng.seed(std::time(nullptr));
        return m_dist(s_rng);
    }

    auto operator()() { return this->GetNext(); }
private:
    static thread_local std::mt19937 s_rng;
    static thread_local constinit bool s_init;
private:
    //static_assert(std::is_integral_v<value_type>);
    //using DistType = std::uniform_real_distribution<float>;
    using DistType = std::conditional_t<std::is_integral_v<value_type>, std::uniform_int_distribution<value_type>, std::uniform_real_distribution<value_type>>;
    DistType m_dist;
}; // class Rng

template<typename T>
requires std::is_arithmetic_v<T>
thread_local std::mt19937 Rng<T>::s_rng;

template<typename T>
requires std::is_arithmetic_v<T>
constinit thread_local bool Rng<T>::s_init = false;

} // namespace eng
