#pragma once

namespace eng::detail {

template<typename T>
union PoolEntry;

template<typename T>
struct BlockHeader {
    PoolEntry<T>* p_prev;
    size_t n;
};

template<typename T>
union PoolEntry {
    explicit constexpr PoolEntry(const BlockHeader<T>& hdr) : hdr(hdr) {}
    explicit constexpr PoolEntry(PoolEntry<T>* next_free) : next_free(next_free) {}
    
    BlockHeader<T> hdr;
    T obj;
    PoolEntry<T>* next_free;
};

template<typename T>
class PoolObjectHolder {
public:
    constexpr PoolObjectHolder(std::nullptr_t) : m_entry(nullptr) {}

    template<typename Self>
    [[nodiscard]] constexpr auto Get(this Self&& self) {
        return &self.m_entry->obj;
    }

    constexpr bool IsValid() const noexcept { return m_entry == nullptr; }

    template<typename Self>
    [[nodiscard]] constexpr auto& operator*(this Self&& self) {
        return self.m_entry->obj;
    } 

    template<typename Self>
    [[nodiscard]] constexpr auto operator->(this Self&& self) {
        return self.Get();
    }
private:
    template<typename S, typename Allocator>
    friend class ObjectPoolImplBase;

    template<typename S>
    friend class PoolObjectHolder;

    friend constexpr bool operator==(PoolObjectHolder<T> lhs, PoolObjectHolder<T> rhs) noexcept;

    using PoolEntry = PoolEntry<T>;

    constexpr PoolObjectHolder(PoolEntry* ent) : m_entry(ent) {}
private:
    PoolEntry* m_entry;
}; // class PoolObjectHolder

template<typename T>
constexpr bool operator==(PoolObjectHolder<T> lhs, std::nullptr_t) noexcept { return lhs.IsValid(); }

template<typename T>
constexpr bool operator==(std::nullptr_t, PoolObjectHolder<T> rhs) noexcept { return rhs == nullptr; }

template<typename T>
constexpr bool operator==(PoolObjectHolder<T> lhs, PoolObjectHolder<T> rhs) noexcept {
    return lhs.m_entry == rhs.m_entry;
}


} // namespace eng::detail
