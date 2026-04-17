#pragma once
#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>

#include <print>

#include <engine/detail/ObjectPoolImpl.h>
#include <engine/detail/ControlBlock.h>

namespace eng {

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectManager : public detail::IControlBlockDeallocator, public detail::ObjectPoolImpl<detail::ControlBlockImpl<std::remove_reference_t<T>>, ObjectManager<T, AllocatorType>, AllocatorType> {
public:
    using value_type     = std::remove_reference_t<T>;
    using reference_type = value_type*;
    using pointer_type   = value_type&;
private:
    using control_blk      = detail::ControlBlock;
    using control_blk_impl = detail::ControlBlockImpl<value_type>;
    using base_type        = detail::ObjectPoolImpl<detail::ControlBlockImpl<std::remove_reference_t<T>>, ObjectManager<T, AllocatorType>, AllocatorType>;
public:
    constexpr ObjectManager() = default;

    constexpr virtual ~ObjectManager() = default;

    [[nodiscard]] constexpr auto GetActiveObjectCount() const { return m_active.size(); }

    template<typename F, typename... Args>
    constexpr void InvokeOnAll(F&& f, Args&&... args) {
        /* Invoke the function on each active object. */
        for (auto& blk : m_active)
            std::invoke(f, &blk->val, std::forward<Args>(args)...);
    }

    template<typename... Args>
    [[nodiscard]] constexpr auto CreateObject(Args&&... args);

private:
    virtual constexpr void Free(control_blk* p_ctrl) override {
        auto p_impl = static_cast<control_blk_impl*>(p_ctrl);

        /* Free the object. */
        base_type::Free(p_impl);

        /* Remove it from the free list*/
        m_active.erase(std::ranges::find(m_active, p_impl));
    }
private:
    friend class detail::ControlBlock;

    std::vector<control_blk_impl*> m_active;
}; // class ObjectManager

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

    /********************************************
     * Standard constructors.                   *
     ********************************************/
    constexpr Handle() noexcept : m_ctlr_blk_ptr(nullptr) {}

    constexpr Handle(std::nullptr_t) noexcept : m_ctlr_blk_ptr(nullptr) {}

    /********************************************
     * Copy & Move constructors.                *
     ********************************************/
    constexpr Handle(const Handle<T>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }

    constexpr Handle(Handle<T>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr Handle(const Handle<OtherType>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }
    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr Handle(Handle<OtherType>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    constexpr ~Handle() { this->Release(); }

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

    constexpr void Destroy() { this->Release(); }

    constexpr auto ToWeak() const noexcept;

    [[nodiscard]] constexpr bool IsValid() const noexcept { return m_ctlr_blk_ptr != nullptr; }

    constexpr operator bool() const noexcept { return this->IsValid(); }

    /********************************************
     * Assignment operators.                    *
     ********************************************/
    constexpr Handle<T>& operator=(const Handle<T>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    constexpr Handle<T>& operator=(Handle<T>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr Handle<T>& operator=(const Handle<OtherType>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr Handle<T>& operator=(Handle<OtherType>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    /********************************************
     * Comparison operators.                    *
     ********************************************/
    template<typename T1, typename T2>
    friend constexpr bool operator==(const Handle<T1>& lhs, const Handle<T2>& rhs) noexcept;

    template<typename T1, typename T2>
    friend constexpr bool operator==(const WeakHandle<T1>& lhs, const Handle<T2>& rhs) noexcept;

    template<typename T1, typename T2>
    friend constexpr bool operator==(const Handle<T1>& lhs, const WeakHandle<T2>& rhs) noexcept;

private:
    using control_blk = detail::ControlBlock;

    template<typename U>
    friend class Handle;

    template<typename U>
    friend class WeakHandle;

    template<typename U, typename Allocator>
    friend class detail::ObjectManager;

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
            this->Release();

        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    constexpr Handle(control_blk* ctlr_blk) noexcept : m_ctlr_blk_ptr(ctlr_blk) {
        /* Increment ref count. */
        m_ctlr_blk_ptr->ref_cnt.Increment();
    }

    constexpr void Release() {
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->ReleaseOne();
    }
private:
    control_blk* m_ctlr_blk_ptr;
}; // class Handle

template<typename T>
class WeakHandle {
public:
    using value_type     = std::remove_reference_t<T>;
    using pointer_type   = value_type*;
    using reference_type = value_type&;

    /********************************************
     * Standard constructors.                   *
     ********************************************/
    constexpr WeakHandle() noexcept : m_ctlr_blk_ptr(nullptr) {}

    constexpr WeakHandle(std::nullptr_t) noexcept : m_ctlr_blk_ptr(nullptr) {}

    /********************************************
     * Construct from weak constructors.        *
     ********************************************/
    constexpr WeakHandle(const WeakHandle<T>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }

    constexpr WeakHandle(WeakHandle<T>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr WeakHandle(const WeakHandle<OtherType>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFrom(other);
    }
    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr WeakHandle(WeakHandle<OtherType>&& other) : m_ctlr_blk_ptr(nullptr) {
        this->MoveFrom(std::move(other));
    }

    /********************************************
     * Construct from strong constructors.      *
     ********************************************/
    explicit constexpr WeakHandle(const Handle<T>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFromStrong(other);
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    explicit constexpr WeakHandle(const Handle<OtherType>& other) : m_ctlr_blk_ptr(nullptr) {
        this->CopyFromStrong(other);
    }

    constexpr ~WeakHandle() { this->Release(); }

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
        return WeakHandle<T>(*this);
    }

    [[nodiscard]] constexpr auto GetRefCount() const noexcept {
        return m_ctlr_blk_ptr->ref_cnt.GetCount();
    }

    constexpr void Destroy() { this->Release(); }

    [[nodiscard]] constexpr bool IsValid() const noexcept { return m_ctlr_blk_ptr != nullptr; }

    constexpr operator bool() const noexcept { return this->IsValid(); }

    /********************************************
     * Assignment from weak Handle.             *
     ********************************************/
    constexpr WeakHandle<T>& operator=(const WeakHandle<T>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    constexpr WeakHandle<T>& operator=(WeakHandle<T>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr WeakHandle<T>& operator=(const WeakHandle<OtherType>& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    //template<std::derived_from<value_type> OtherType>
    template<typename OtherType>
    constexpr WeakHandle<T>& operator=(WeakHandle<OtherType>&& rhs) {
        this->MoveFrom(std::move(rhs));
        return *this;
    }

    /********************************************
     * Comparison operators.                    *
     ********************************************/
    template<typename T1, typename T2>
    friend constexpr bool operator==(const WeakHandle<T1>& lhs, const WeakHandle<T2>& rhs) noexcept;

    template<typename T1, typename T2>
    friend constexpr bool operator==(const WeakHandle<T1>& lhs, const Handle<T2>& rhs) noexcept;

    template<typename T1, typename T2>
    friend constexpr bool operator==(const Handle<T1>& lhs, const WeakHandle<T2>& rhs) noexcept;
private:
    using control_blk = detail::ControlBlock;

    template<typename U>
    friend class WeakHandle;

    template<typename U>
    friend class Handle;

    template<typename U, typename Allocator>
    friend class detail::ObjectManager;

    friend struct std::hash<WeakHandle<value_type>>;

    template<typename OtherType>
    constexpr void CopyFrom(const WeakHandle<OtherType>& other) {
        auto old = this->m_ctlr_blk_ptr;

        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;

        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->AddWeak();
        if (old != nullptr)
            old->ReleaseWeak();
    }

    template<typename OtherType>
    constexpr void MoveFrom(WeakHandle<OtherType>&& other) {
        if (this->m_ctlr_blk_ptr != other.m_ctlr_blk_ptr)
            this->Release();

        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
        other.m_ctlr_blk_ptr = nullptr;
    }

    template<typename OtherType>
    constexpr void CopyFromStrong(const Handle<OtherType>& other) {
        auto old = this->m_ctlr_blk_ptr;

        m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;

        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->AddWeak();
        //if (old != nullptr)
        //    old->ReleaseWeak();
    }

    constexpr WeakHandle(control_blk* ctlr_blk) noexcept : m_ctlr_blk_ptr(ctlr_blk) {
        /* Increment ref count. */
        m_ctlr_blk_ptr->weak_ref_cnt.Increment();
    }

    constexpr void Release() {
        if (m_ctlr_blk_ptr != nullptr)
            m_ctlr_blk_ptr->ReleaseWeak();
    }
private:
    control_blk* m_ctlr_blk_ptr;
}; // class WeakHandle

template<typename T>
constexpr auto Handle<T>::ToWeak() const noexcept { return WeakHandle(*this); }

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const Handle<T1>& lhs, const Handle<T2>& rhs) noexcept {
    return lhs.m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
}

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const WeakHandle<T1>& lhs, const WeakHandle<T2>& rhs) noexcept {
    return lhs.m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
}

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const Handle<T1>& lhs, const WeakHandle<T2>& rhs) noexcept {
    return lhs.m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
}

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const WeakHandle<T1>& lhs, const Handle<T2>& rhs) noexcept {
    return lhs.m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
}

// This needs to be after the Handle class def so it can see the complete class.
template<typename T, typename Allocator>
template<typename... Args>
[[nodiscard]] constexpr auto ObjectManager<T, Allocator>::CreateObject(Args&&... args) {
    /* Allocate a new object / control block. */
    auto blk = base_type::CreateObject(this, std::forward<Args>(args)...);

    /* Add it to the active list. */
    m_active.push_back(blk);

    return Handle<value_type>(blk);
}

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

