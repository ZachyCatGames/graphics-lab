#pragma once
#include "ControlBlock.h"

namespace eng::detail {

template<typename T>
class HandleConstructor {
public:
    static constexpr Handle<T> ConstructFromControlBlockPointer(detail::TypedControlBlock<T>* pControlBlock, T* pValue) {
        return Handle<T>(static_cast<ControlBlock*>(pControlBlock), pValue);
    }
}; // class HandleConstructor

} // namespace eng::detail