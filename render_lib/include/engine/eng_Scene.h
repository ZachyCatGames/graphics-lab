#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
#include <vector>

#include <engine/eng_ICamera.h>
#include <engine/eng_Interval.h>
#include <engine/eng_IShader.h>
#include <engine/eng_IShape.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Rng.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng {

struct RtSceneConfig {
    int recursionDepth;

    constexpr RtSceneConfig& SetRecusionDepth(int depth) noexcept {
        recursionDepth = depth;
        return *this;
    }
};

struct SceneConfig {
    union {
        RtSceneConfig rtConfig;
    };
};

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

        [[nodiscard]] constexpr auto GetShader() const { return this->GetAttribs()->shader; }
        [[nodiscard]]  auto GetHandle() const { return std::get<0>(*m_iter); }

        [[nodiscard]] constexpr auto IsValid() const { return this->GetHandle().IsValid(); }
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

    /**************************************************
     * Shape functions.
     **************************************************/
    /**
     * Initializes a shape of type T with the provided args and
     * adds it to the scene.
     * 
     * An ObjectContext object is returned that can be used to setup
     * additional object state (e.g., shader) by calling its methods
     * (e.g., BindShader).
     * This context object cannot be assigned to a variable and may
     * be invalidated as soon as another shape is added to the scene.
     * 
     * @param args  Arguments forwarded to the T to be constructed
     * @return Object context.
     */
    template<std::derived_from<IShape> T, typename... Args>
    ObjectContext EmplaceShape(Args&&... args) {
        auto handle = T::Create(std::forward<Args>(args)...);
        return this->InsertShape(handle);
    }

    /**
     * Add an initialized shape to the scene.
     * 
     * @param handle  Handle of the shape to be added
     * @return Object context (see \ref EmplaceShape).
     */
    ObjectContext InsertShape(Handle<IShape> handle);

    [[nodiscard]] bool ContainsShape(Handle<IShape> shape);

    ObjectContext GetShapeContext(Handle<IShape> shape);

    void RemoveShape(Handle<IShape> shape);

    /**************************************************
     * Point light functions.
     **************************************************/
    shdr::PointLight& EmplacePointLight(const Vector3DF& pos, const Vector3DF& intensity);

    shdr::PointLight& InsertPointLight(const shdr::PointLight& light);
    //void InsertPointLight(shdr::PointLight&& light);

    const std::vector<shdr::PointLight>& GetPointLights() const noexcept { return m_lights; }

    /**
     * Reserve / preallocate shape handles.
     * 
     * This can be used when the number of shapes being used is known ahead
     * of time to reduce the number of allocations / copies required from
     * vector resizes.
     * 
     * @param count  Number of shape handles to preallocate.
     */
    void ReserveShapes(size_t count);

    /** 
     * Reserve / preallocate point light objects.
     * 
     * @param count  Number of point lights to preallocate.
     */
    void ReservePointLights(size_t count);
protected:
    /**************************************************
     * Init functions.
     **************************************************/
    Scene();
private:
protected:
    std::vector<Handle<IShape>> m_shapes;
    MapType m_attribs;

    std::vector<shdr::PointLight> m_lights;

    bool m_objectsUpdated;
}; // class Scene

} // namespace eng
