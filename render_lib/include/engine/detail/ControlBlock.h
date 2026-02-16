#pragma once
#include <engine/eng_ReferenceCount.h>
#include "ObjectPoolTypes.h"

namespace eng::detail {

struct ControlBlock;

class IControlBlockDeallocator {
private:
    friend struct ControlBlock;
    virtual constexpr void Free(ControlBlock* p_blk) = 0;
}; // class IControlBlockDeallocator

struct ControlBlock {
    constexpr ControlBlock(IControlBlockDeallocator* man, void* p_val) : ref_cnt(), manager(man), p_val(p_val) {};
    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) = delete;

    template<typename T>
    [[nodiscard]] constexpr auto Get() const { return static_cast<T*>(p_val); }

    constexpr void AddOne() { ref_cnt.Increment(); }

    constexpr void ReleaseOne() {
        if (ref_cnt.Decrement())
            manager->Free(this); // we need to be careful to no operate on ourself after this
        return; // explicit about this...
    }

    ReferenceCount ref_cnt;
    IControlBlockDeallocator* manager;
    void* p_val;
}; // struct ControlBlock

template<typename T>
struct ControlBlockImpl : public ControlBlock {
    template<typename... Args>
    constexpr ControlBlockImpl(IControlBlockDeallocator* man, Args&&... args) : ControlBlock(man, &val), pool_hndl(nullptr), val(std::forward<Args>(args)...) {}

    PoolObjectHolder<ControlBlockImpl<T>> pool_hndl;
    T val;
}; // struct ControlBlockImpl

} // namespace eng::detail
