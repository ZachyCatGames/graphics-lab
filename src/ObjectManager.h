#pragma once
#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>

#include "detail/ObjectPoolImpl.h"
#include "detail/ControlBlock.h"

namespace eng {

template<typename T, typename AllocatorType = std::allocator<T>>
class ObjectManager : public detail::ObjectPoolImpl<detail::ControlBlock<std::remove_reference_t<T>>, ObjectManager<T, AllocatorType>, AllocatorType> {
public:
    using value_type     = detail::ObjectPoolImpl<T, ObjectManager, AllocatorType>::value_type;
    using reference_type = detail::ObjectPoolImpl<T, ObjectManager, AllocatorType>::reference_type;
    using pointer_type   = detail::ObjectPoolImpl<T, ObjectManager, AllocatorType>::pointer_type;
private:
    using control_blk = detail::ControlBlock<value_type>;
    using base_type   = detail::ObjectPoolImpl<detail::ControlBlock<std::remove_reference_t<T>>, ObjectManager<T, AllocatorType>, AllocatorType>;
public:
    /**
     * Object handle.
     * 
     * The ObjectPool class returns object handles when allocating objects.
     * These function similar to shared pointers in that they use reference counting and
     * destroy/free the pointed-to object once no references remain.
     * 
     * Handles can be duplicated (Clone), explicitly cleared (Destroy), and moved (move constructor).
     */
    class Handle {
    public:
        constexpr Handle(const Handle& other) {
            this->Free();
            m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
            m_ctlr_blk_ptr->ref_cnt.Increment();
        }
    
        constexpr Handle(Handle&& other) {
            this->Free();
            m_ctlr_blk_ptr = other.m_ctlr_blk_ptr;
            other.m_ctlr_blk_ptr = nullptr;
        }
    
        constexpr ~Handle() {
            this->Free();
        }
    
        template<typename Self>
        [[nodiscard]] constexpr auto& operator*(this Self&& self) {
            return self.m_ctlr_blk_ptr->val;
        } 
    
        template<typename Self>
        [[nodiscard]] constexpr auto operator->(this Self&& self) {
            return &self.m_ctlr_blk_ptr->val;
        }
    
        template<typename Self>
        [[nodiscard]] constexpr auto Get(this Self&& self) {
            return &self.m_ctlr_blk_ptr->val;
        }
    
        [[nodiscard]] constexpr auto Clone() const {
            return Handle(*this);
        }

        [[nodiscard]] constexpr auto GetRefCount() const {
            return m_ctlr_blk_ptr->ref_cnt.GetCount();
        }
    
        constexpr void Destroy() {
            m_ctlr_blk_ptr->ref_cnt.Decrement();
            m_ctlr_blk_ptr = nullptr;
        }

        constexpr Handle& operator=(const Handle& rhs) {
            this->Free();
            m_ctlr_blk_ptr = rhs.m_ctlr_blk_ptr;
            m_ctlr_blk_ptr->ref_cnt.Increment();
            return *this;
        }

        constexpr Handle& operator=(Handle&& rhs) {
            this->Free();
            m_ctlr_blk_ptr = rhs.m_ctlr_blk_ptr;
            rhs.m_ctlr_blk_ptr = nullptr;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const Handle& rhs) {
            return this->m_ctlr_blk_ptr == rhs.m_ctlr_blk_ptr;
        }
    private:
        using Manager = ObjectManager<T, AllocatorType>;
        friend class ObjectManager<T, AllocatorType>;
    
        constexpr Handle(control_blk* ctlr_blk) : m_ctlr_blk_ptr(ctlr_blk) {
            /* Increment ref count. */
            m_ctlr_blk_ptr->ref_cnt.Increment();
        }

        constexpr void Free() {
            if (m_ctlr_blk_ptr != nullptr && m_ctlr_blk_ptr->ref_cnt.Decrement())
                Manager::Get()->Free(m_ctlr_blk_ptr);
        }
    private:
        control_blk* m_ctlr_blk_ptr;
    }; // class Handle
public:
    [[nodiscard]] static constexpr auto Get() {
        if (!s_manager.IsInitialized()) {
            s_manager.Initialize();
        }

        return &s_manager;
    }

    [[nodiscard]] constexpr auto GetActiveObjectCount() const { return m_active.size(); }

    template<typename F, typename... Args>
    constexpr void InvokeOnAll(F&& f, Args&&... args) {
        /* Invoke the function on each active object. */
        for (auto& obj : m_active)
            std::invoke(f, &obj->obj.val, std::forward<Args>(args)...);
    }


    template<typename... Args>
    [[nodiscard]] constexpr Handle CreateObject(Args&&... args) {
        /* Allocate a new object / control block. */
        auto blk = base_type::CreateObject(std::forward<Args>(args)...);

        /* Add it to the active list. */
        m_active.push_back(blk);

        return Handle(blk);
    }
private:
    constexpr ObjectManager() = default;

    constexpr void Free(control_blk* p_obj) {
        /* Free the object. */
        base_type::Free(p_obj);

        /* Remove it from the free list*/
        m_active.erase(std::ranges::find(m_active, p_obj));
    }
private:
    static constinit ObjectManager s_manager;

    std::vector<control_blk*> m_active;
}; // class ObjectManager

template<typename T, typename AllocatorType>
constinit ObjectManager<T, AllocatorType> ObjectManager<T, AllocatorType>::s_manager{};

template<typename T, typename AllocatorType>
using Handle = typename ObjectManager<T, AllocatorType>::Handle;

} // namespace eng
