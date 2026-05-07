#pragma once
#include <cstddef>
#include <string>
#include <string_view>

namespace eng {

/* Use std::hash by default. */
template<typename T>
struct Hash : std::hash<T> {};

namespace detail {

/*
 * djb2 hash algorithm, found via:
 * http://www.cse.yorku.ca/~oz/hash.html
 * https://stackoverflow.com/questions/7968674/unexpected-collision-with-stdhash
*/
template<typename StringType>
constexpr size_t HashStringFunction(const StringType& str) noexcept {
    size_t hash = 5381;
    for(auto c : str) {
        hash = (hash * 33) ^ c;
    }

    return hash;
}

} // namespace detail

template<typename T>
struct Hash<std::basic_string_view<T>> {
    /**
     * @param str  String to hash
     * @return Hash
     */
    constexpr size_t operator()(const std::basic_string_view<T>& str) const {
        return detail::HashStringFunction(str);
    }
}; // struct Hash<std::basic_string_view<T>>

template<typename T>
struct Hash<std::basic_string<T>> {
    /**
     * @param str  String to hash
     * @return Hash
     */
    constexpr size_t operator()(const std::basic_string<T>& str) const {
        return detail::HashStringFunction(str);
    }
}; // struct Hash<std::basic_string<T>>

} // namespace eng
