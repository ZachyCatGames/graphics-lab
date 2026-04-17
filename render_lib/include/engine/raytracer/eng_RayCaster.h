#pragma once
#include <engine/eng_Scene.h>
#include <engine/eng_Ray.h>
#include <engine/eng_RenderObject.h>
#include <engine/raytracer/eng_Bvh.h>

namespace eng::rt {

class RayCaster {
public:
    RayCaster() : m_scene(nullptr) {}
    RayCaster(std::vector<RenderObject>* ro, int maxDepth) :
        m_pRO(ro),
        m_maxDepth(maxDepth) 
    {
        assert(ro);
    }

    int Initialize(std::vector<RenderObject>* ro, int maxDepth) {
        if(!ro)
            return -1;
        m_pRO = ro;
        return 0;
    }

    void Finalize() { m_pRO = nullptr; }

    [[nodiscard]] bool IsInitialized() const noexcept { return m_pRO != nullptr; }

    void PrepareBvhTree();

    Vector3DF CastRayIntoScene(const Ray& r, Interval<float> t_range, int depth);

    bool IsObjectInPath(const Ray& r, Interval<float> t_range); 
private:
    std::vector<RenderObject>* m_pRO;
    Bvh m_Bvh;
    int m_maxDepth;
}; // class RayCaster

} // namespace eng::rt
