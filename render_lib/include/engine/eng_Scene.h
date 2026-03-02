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

    /**
     * Initialize a scene with default parameters.
     * 
     * The defaults are as follows:
     *  - max depth of 4
     *  - 1 sample per pixel
     *  - no sample randomization
     */
    Scene();

    /**
     * Initializes a scene with the provided parameters.
     * 
     * @param max_depth  max number of ray reflections
     * @param samples_per_pixel  number of color samples taken per-pixel (for AA)
     * @param random_samples  enable randomized sample locations
     */
    Scene(int max_depth, int samples_per_pixel, bool random_samples);

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

    [[nodiscard]] bool Contains(Handle<IShape> shape);

    ObjectContext GetContext(Handle<IShape> shape);

    void Remove(Handle<IShape> shape);

    Vector3DF GetRayColor(const Ray& r, Interval<float> t_range, int depth);
    Vector3DF GetPixelColor(ICamera* p_cam, int x, int y);

    bool ObjectInPath(const Ray& r, Interval<float> t_range); 
private:
    std::vector<Handle<IShape>> m_shapes;
    MapType m_attribs;

    int m_max_depth;
    int m_samples_per_pixel = 4;
    bool m_randomize_pixel_samples = true;
}; // class Scene

} // namespace eng
