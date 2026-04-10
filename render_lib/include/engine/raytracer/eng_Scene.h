#pragma once
#include <engine/eng_Scene.h>
#include <engine/raytracer/eng_Bvh.h>

namespace eng::rt {

class Scene : public eng::Scene {
public:
    void PrepareBvhTree();

    Vector3DF GetRayColor(const Ray& r, Interval<float> t_range, int depth);

    bool IsObjectInPath(const Ray& r, Interval<float> t_range); 
private:
    Bvh m_shapeBvh;
    bool m_bvhAutoUpdate;
}; // class Scene

} // namespace eng::rt