#pragma once
#include <engine/eng_ReferenceCount.h>
#include "eng_ObjectPoolTypes.h"

namespace eng::detail {

struct ControlBlock;

class IControlBlockDeallocator {
private:
    friend struct ControlBlock;
    virtual constexpr void Free(ControlBlock* p_blk) = 0;
}; // class IControlBlockDeallocator

struct ControlBlock {
    constexpr ControlBlock(IControlBlockDeallocator* man) noexcept
        : ref_cnt(),
          manager(man) {};
    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) = delete;

    virtual ~ControlBlock() = default;

    constexpr void AddOne() noexcept { ref_cnt.Increment(); }

    constexpr void ReleaseOne() {
        if (ref_cnt.Decrement())
            manager->Free(this); // we need to be careful to no operate on ourself after this
        return; // explicit about this...
    }

    ReferenceCount ref_cnt;
    IControlBlockDeallocator* manager;
}; // struct ControlBlock

template<typename T>
struct TypedControlBlock : public ControlBlock {
    template<typename... Args>
    constexpr TypedControlBlock(IControlBlockDeallocator* man, Args&&... args) :
        ControlBlock(man),
        val(std::forward<Args>(args)...) {}

    virtual ~TypedControlBlock() = default;

    T val;
}; // struct TypedControlBlock

} // namespace eng::detail
