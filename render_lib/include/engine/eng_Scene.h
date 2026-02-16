#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
#include <vector>
#include "shapes/IShape.h"
#include "shaders/IShader.h"
#include "ICamera.h"
#include "ObjectManager.h"

namespace eng {

class Scene {
private:
    struct ShapeAttributes {
        constexpr ShapeAttributes() : shader() {}

        Handle<IShader> shader;
    }; // struct ShapeAttributes;

    using MapType  = std::unordered_map<Handle<IShape>, ShapeAttributes>;
    using IterType = MapType::iterator;
public:
    class ObjectContext {
    public:
        ObjectContext() = delete;
        ObjectContext(const ObjectContext&) = delete;
        ObjectContext(ObjectContext&&) = delete;

        constexpr const ObjectContext& BindShader(Handle<IShader> shader) const {
            this->GetAttribs()->shader = shader;
            return *this;
        }

        constexpr auto GetShader() const { return this->GetAttribs()->shader; }
        constexpr auto GetHandle() const { return std::get<0>(*m_iter); }

        constexpr auto IsValid() const { return this->GetHandle().IsValid(); }
    private:
        friend class Scene;
        constexpr ObjectContext(IterType iter) : m_iter(iter), valid(true) {}

        constexpr ObjectContext(IterType iter, bool) : m_iter(iter), valid(false) {} 

        template<typename Self>
        constexpr ShapeAttributes* GetAttribs(this Self&& self) { return &std::get<1>(*self.m_iter); }
    private:
        IterType m_iter;
        bool valid;
    }; // class ObjectContext

    template<std::derived_from<IShape> T, typename... Args>
    ObjectContext EmplaceShape(Args&&... args) {
        auto handle = T::Create(std::forward<Args>(args)...);
        return this->InsertShape(handle);
    }

    ObjectContext InsertShape(Handle<IShape> handle);

    bool Contains(Handle<IShape> shape);

    ObjectContext GetContext(Handle<IShape> shape);

    void Remove(Handle<IShape> shape);

    Vector3DF GetPixelColor(ICamera* p_cam, int x, int y);
private:
    std::vector<Handle<IShape>> m_shapes;
    MapType m_attribs;
}; // class Scene

} // namespace eng
