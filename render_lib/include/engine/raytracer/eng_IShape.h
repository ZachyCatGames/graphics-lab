#pragma once
#include <engine/raytracer/eng_HitStruct.h>
#include <engine/eng_Handle.h>
#include <engine/eng_IShape.h>
#include <engine/eng_Interval.h>
#include <engine/eng_Ray.h>
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt {

class IShape : public eng::IShape {
public:
    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const = 0;

    [[nodiscard]] Handle<IShader> GetShaderRT() const { return this->GetShader().StaticCast<IShader>(); };
}; // class IShape

} // namespace eng::shape
