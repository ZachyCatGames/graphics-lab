#pragma once
#include <cstddef>
#include <string_view>

namespace eng {

/* Use std::hash by default. */
template<typename T>
struct Hash : std::hash<T> {};

template<>
struct Hash<std::string_view> {
    constexpr size_t operator()(const std::string_view& str) const {
        size_t hash = 5381;
        for(auto c : str) {
            hash = (hash * 33) ^ c;
        }

        return hash;
    }
}; // struct Hash<std::string_view>

} // namespace eng
