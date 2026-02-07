#pragma once
#include "../ReferenceCount.h"

namespace eng::detail {

template<typename T>
struct ControlBlock {
    template<typename... Args>
    constexpr ControlBlock(Args&&... args) : ref_cnt(), val(std::forward<Args>(args)...) {}

    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) = delete;

    ReferenceCount ref_cnt;
    T val;
}; // struct ControlBlock

} // namespace eng::detail
