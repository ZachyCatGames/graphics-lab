#pragma once
#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>

#include <print>

#include <engine/detail/ObjectPoolImpl.h>
#include <engine/detail/ControlBlock.h>
#include <engine/eng_ManagedPoolAllocator.h>
#include <engine/eng_Handle.h>
#include <engine/detail/eng_HandleConstructor.h>

namespace eng {

namespace detail {

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectManagerImpl : public detail::IControlBlockDeallocator {
public:
    using value_type     = std::remove_reference_t<T>;
    using reference_type = value_type*;
    using pointer_type   = value_type&;

private:
    using PoolType = ManagedPoolAllocator<detail::TypedControlBlock<std::remove_reference_t<T>>, AllocatorType>;

    using control_blk       = detail::ControlBlock;
    using typed_control_blk = detail::TypedControlBlock<value_type>;
public:
    [[nodiscard]] constexpr auto GetActiveObjectCount() const { return m_active.size(); }

    void Initialize(size_t initialObjectCount = 4) {
        m_Allocator.Initialize(initialObjectCount);
    }

    bool IsInitialized() const noexcept { return m_Allocator.IsInitialized(); }

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
        auto pTyped = static_cast<typed_control_blk*>(p_ctrl);

        /* Destroy and free the object. */
        m_Allocator.DestroyAndFree(pTyped);

        /* Remove it from the free list*/
        m_active.erase(std::ranges::find(m_active, pTyped));
    }
private:
    friend class detail::ControlBlock;

    PoolType m_Allocator;
    std::vector<typed_control_blk*> m_active;
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

namespace detail {

template<typename T, typename Allocator>
template<typename... Args>
[[nodiscard]] constexpr auto ObjectManagerImpl<T, Allocator>::CreateObject(Args&&... args) {
    /* Allocate a new object / control block. */
    typed_control_blk* blk = m_Allocator.AllocateAndConstruct(this, std::forward<Args>(args)...);

    /* Add it to the active list. */
    m_active.push_back(blk);

    return detail::HandleConstructor<value_type>::ConstructFromControlBlockPointer(blk);
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

