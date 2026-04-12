#pragma once
#include <cstddef>
#include <string_view>

namespace eng {

/* Use std::hash by default. */
template<typename T>
struct Hash : std::hash<T> {};

template<>
struct Hash<std::string_view> {
    /**
     * djb2 hash algorithm, found via:
     * http://www.cse.yorku.ca/~oz/hash.html
     * https://stackoverflow.com/questions/7968674/unexpected-collision-with-stdhash
     * 
     * @param str  String to hash
     * @return Hash
     */
    constexpr size_t operator()(const std::string_view& str) const {
        size_t hash = 5381;
        for(auto c : str) {
            hash = (hash * 33) ^ c;
        }

        return hash;
    }
}; // struct Hash<std::string_view>

} // namespace eng
