#pragma once
#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>

#include <print>

#include <engine/detail/ObjectPoolImpl.h>
#include <engine/detail/ControlBlock.h>

namespace eng {

namespace detail {

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectManagerImpl : public detail::IControlBlockDeallocator, public detail::ObjectPoolImpl<detail::ControlBlockImpl<std::remove_reference_t<T>>, ObjectManagerImpl<T, AllocatorType>, AllocatorType> {
public:
    using value_type     = std::remove_reference_t<T>;
    using reference_type = value_type*;
    using pointer_type   = value_type&;
private:
    using control_blk      = detail::ControlBlock;
    using control_blk_impl = detail::ControlBlockImpl<value_type>;
    using base_type        = detail::ObjectPoolImpl<detail::ControlBlockImpl<std::remove_reference_t<T>>, ObjectManagerImpl<T, AllocatorType>, AllocatorType>;
public:
    [[nodiscard]] constexpr auto GetActiveObjectCount() const { return m_active.size(); }

    template<typename F, typename... Args>
    constexpr void InvokeOnAll(F&& f, Args&&... args) {
        /* Invoke the function on each active object. */
        for (auto& blk : m_active)
            std::invoke(f, &blk->val, std::forward<Args>(args)...);
    }

    template<typename... Args>
    [[nodiscard]] constexpr auto CreateObject(Args&&... args);
protected:
    constexpr ObjectManagerImpl() = default;
private:
    virtual constexpr void Free(control_blk* p_ctrl) override {
        auto p_impl = static_cast<control_blk_impl*>(p_ctrl);

        /* Free the object. */
        base_type::Free(p_impl->pool_hndl);

        /* Remove it from the free list*/
        m_active.erase(std::ranges::find(m_active, p_impl));
    }
private:
    friend class detail::ControlBlock;

    std::vector<control_blk_impl*> m_active;
}; // class ObjectManagerImpl

} // namespace detail

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectManager : public detail::ObjectManagerImpl<T, AllocatorType> {
public:
    [[nodiscard]] static constexpr auto Get() {
        if (!s_manager.IsInitialized()) {
            s_manager.Initialize();
        }

        return &s_manager;
    }
private:
    static constinit ObjectManager s_manager;
}; // class ObjectManager

template<typename T, typename AllocatorType>
constinit ObjectManager<T, AllocatorType> ObjectManager<T, AllocatorType>::s_manager{};

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

    constexpr Handle() noexcept : m_ctlr_blk_ptr(nullptr) {}

    constexpr Handle(std::nullptr_t) noexcept : m_ctlr_blk_ptr(nullptr) {}

    constexpr Handle(const Handle<T>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }

    constexpr Handle(Handle<T>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    template<std::derived_from<value_type> OtherType>
    constexpr Handle(const Handle<OtherType>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }
    template<std::derived_from<value_type> OtherType>
    constexpr Handle(Handle<OtherType>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    constexpr ~Handle() { this->Free(); }

    template<typename Self>
    [[nodiscard]] constexpr auto& operator*(this Self&& self) noexcept {
        return *self.m_ctlr_blk_ptr->template Get<value_type>();
    } 

    template<typename Self>
    [[nodiscard]] constexpr auto operator->(this Self&& self) noexcept {
        return self.Get();
    }

    template<typename Self>
    [[nodiscard]] constexpr auto Get(this Self&& self) noexcept {
        return self.m_ctlr_blk_ptr->template Get<value_type>();
    }

    [[nodiscard]] constexpr auto Clone() const {
        return Handle<T>(*this);
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

    template<typename U, typename Allocator>
    friend class detail::ObjectManagerImpl;

    friend struct std::hash<Handle<value_type>>;

    template<typename OtherType>
    constexpr void CopyFrom(const Handle<OtherType>& other) {
        auto old = this->m_ctlr_blk_ptr;

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

        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    constexpr Handle(control_blk* ctlr_blk) noexcept : m_ctlr_blk_ptr(ctlr_blk) {
        /* Increment ref count. */
        m_ctlr_blk_ptr->ref_cnt.Increment();
    }

    constexpr void Free() {
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->ReleaseOne();
    }
private:
    control_blk* m_ctlr_blk_ptr;
}; // class Handle

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const Handle<T1>& lhs, const Handle<T2>& rhs) noexcept {
    return lhs.m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
}

namespace detail {

template<typename T, typename Allocator>
template<typename... Args>
[[nodiscard]] constexpr auto ObjectManagerImpl<T, Allocator>::CreateObject(Args&&... args) {
    /* Allocate a new object / control block. */
    auto blk = base_type::CreateObject(this, std::forward<Args>(args)...);
    blk->pool_hndl = blk;

    /* Add it to the active list. */
    m_active.push_back(blk.Get());

    return Handle<value_type>(blk.Get());
}

} // namespace detail

} // namespace eng

namespace std {

template<typename T>
struct hash<eng::Handle<T>> {
    constexpr hash() = default;
    constexpr hash(const hash<eng::Handle<T>>& other) = default;

    constexpr size_t operator()(const eng::Handle<T>& hndl) const {
        return hasher(hndl.m_ctlr_blk_ptr);
    }

    std::hash<eng::detail::ControlBlock*> hasher;
}; // struct hash<eng::Handle<T>>

template<typename T>
void swap(std::hash<eng::Handle<T>>& hndl1, std::hash<eng::Handle<T>>& hndl2) {
    auto tmp = hndl1.hasher;
    hndl1.hasher = hndl2.hasher;
    hndl2.hasher = tmp;
}

} // namespace std

