#pragma once
#include "ControlBlock.h"

namespace eng::detail {

template<typename T>
class HandleConstructor {
public:
    static constexpr Handle<T> ConstructFromControlBlockPointer(detail::TypedControlBlock<T>* pControlBlock) {
        return Handle<T>(pControlBlock);
    }
}; // class HandleConstructor

} // namespace eng::detail