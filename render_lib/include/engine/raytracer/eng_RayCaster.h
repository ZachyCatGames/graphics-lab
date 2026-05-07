#pragma once
#include <engine/eng_Scene.h>
#include <engine/eng_Ray.h>
#include <engine/raytracer/eng_Bvh.h>
#include <memory>

namespace eng::rt {

class RayCaster {
public:
    RayCaster() : m_scene(nullptr) {}
    RayCaster(const std::shared_ptr<eng::Scene>& scene, int maxDepth) :
        m_scene(scene),
        m_maxDepth(maxDepth) 
    {
        assert(m_scene);
    }

    int Initialize(const std::shared_ptr<eng::Scene>& scene, int maxDepth) {
        if(!m_scene)
            return -1;
        m_scene = scene;
        return 0;
    }

    void Finalize() { m_scene.reset(); }

    [[nodiscard]] bool IsInitialized() const noexcept { return m_scene != nullptr; }

    std::shared_ptr<Scene> GetScene() const { return m_scene; }

    void PrepareBvhTree();

    Vector3DF CastRayIntoScene(const Ray& r, Interval<float> t_range, int depth);

    bool IsObjectInPath(const Ray& r, Interval<float> t_range); 
private:
    std::shared_ptr<eng::Scene> m_scene;
    Bvh m_Bvh;
    int m_maxDepth;
}; // class RayCaster

} // namespace eng::rt
