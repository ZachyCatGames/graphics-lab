#pragma once
#include <engine/detail/eng_ControlBlock.h>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace eng {

namespace detail {

template<typename T>
class HandleConstructor;

} // namespace detail

/**
 * Object handle.
 * 
 * The ObjectPool class returns object handles when allocating objects.
 * These function similar to shared pointers in that they use reference counting and
 * destroy/free the pointed-to object once no references remain.
 * 
 * Handles can be duplicated (Clone), explicitly cleared (Destroy), and moved (move constructor).
 */
template<typename T>
class Handle {
public:
    using value_type     = std::remove_reference_t<T>;
    using pointer_type   = value_type*;
    using reference_type = value_type&;

    constexpr Handle() noexcept : m_pValue(nullptr), m_ctlr_blk_ptr(nullptr) {}

    constexpr Handle(std::nullptr_t) noexcept : m_pValue(nullptr), m_ctlr_blk_ptr(nullptr) {}

    constexpr Handle(const Handle<T>& other) :
        m_pValue(other.m_pValue),
        m_ctlr_blk_ptr(other.m_ctlr_blk_ptr)
    {
        assert(!m_pValue && !m_ctlr_blk_ptr || m_pValue && m_ctlr_blk_ptr);
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->AddOne();
    }

    constexpr Handle(Handle<T>&& other) :
        m_pValue(other.m_pValue),
        m_ctlr_blk_ptr(other.m_ctlr_blk_ptr)
    {
        other.m_pValue = nullptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    template<std::derived_from<value_type> OtherType>
    constexpr Handle(const Handle<OtherType>& other) :         m_pValue(other.m_pValue),
        m_ctlr_blk_ptr(other.m_ctlr_blk_ptr)
    {
        assert(!m_pValue && !m_ctlr_blk_ptr || m_pValue && m_ctlr_blk_ptr);
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->AddOne();
    }
    
    template<std::derived_from<value_type> OtherType>
    constexpr Handle(Handle<OtherType>&& other) :
        m_pValue(other.m_pValue),
        m_ctlr_blk_ptr(other.m_ctlr_blk_ptr)
    {
        other.m_pValue = nullptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    constexpr ~Handle() { this->Free(); }

    template<typename Self>
    [[nodiscard]] constexpr auto& operator*(this Self&& self) noexcept {
        return *self.m_pValue;
    } 

    template<typename Self>
    [[nodiscard]] constexpr auto operator->(this Self&& self) noexcept {
        return self.Get();
    }

    template<typename Self>
    [[nodiscard]] constexpr pointer_type Get(this Self&& self) noexcept {
        if (self.m_ctlr_blk_ptr == nullptr) return nullptr;
        return self.m_pValue;
    }

    [[nodiscard]] constexpr auto Clone() const {
        return Handle<T>(*this);
    }

    template<typename To>
    [[nodiscard]] constexpr Handle<To> StaticCast() const {
        return Handle<To>(m_ctlr_blk_ptr, static_cast<Handle<To>::pointer_type>(m_pValue));
    }

    template<typename To>
    [[nodiscard]] constexpr Handle<To> DynamicCast() const {
        return Handle<To>(m_ctlr_blk_ptr, dynamic_cast<Handle<To>::pointer_type>(m_pValue));
    }

    [[nodiscard]] constexpr auto GetRefCount() const noexcept {
        return m_ctlr_blk_ptr->ref_cnt.GetCount();
    }

    constexpr void Destroy() { this->Free(); }

    [[nodiscard]] constexpr bool IsValid() const noexcept { return m_ctlr_blk_ptr != nullptr; }

    constexpr operator bool() const noexcept { return this->IsValid(); }

    constexpr Handle<T>& operator=(const Handle<T>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    constexpr Handle<T>& operator=(Handle<T>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    template<std::derived_from<value_type> OtherType>
    constexpr Handle<T>& operator=(const Handle<OtherType>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    template<std::derived_from<value_type> OtherType>
    constexpr Handle<T>& operator=(Handle<OtherType>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    template<typename T1, typename T2>
    friend constexpr bool operator==(const Handle<T1>& lhs, const Handle<T2>& rhs) noexcept;
private:
    using control_blk = detail::ControlBlock;

    template<typename U>
    friend class Handle;

    template<typename U>
    friend class detail::HandleConstructor;

    friend struct std::hash<Handle<value_type>>;

    template<typename OtherType>
    constexpr void CopyFrom(const Handle<OtherType>& other) {
        control_blk* old = this->m_ctlr_blk_ptr;

        m_pValue = other.m_pValue;
        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;

        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->AddOne();
        if (old != nullptr)
            old->ReleaseOne();
    }

    template<typename OtherType>
    constexpr void MoveFrom(Handle<OtherType>&& other) {
        if (this->m_ctlr_blk_ptr != other.m_ctlr_blk_ptr)
            this->Free();

        m_pValue = other.m_pValue;
        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    constexpr Handle(control_blk* ctlr_blk, pointer_type pValue) noexcept :
        m_ctlr_blk_ptr(ctlr_blk),
        m_pValue(pValue)
    {
        /* Increment ref count. */
        m_ctlr_blk_ptr->ref_cnt.Increment();
    }

    constexpr void Free() {
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->ReleaseOne();
    }
private:
    pointer_type m_pValue;
    control_blk* m_ctlr_blk_ptr;
}; // class Handle

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const Handle<T1>& lhs, const Handle<T2>& rhs) noexcept {
    return lhs.m_pValue == rhs.m_pValue;
}

} // namespace eng