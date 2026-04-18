#pragma once
#include <engine/raytracer/eng_HitStruct.h>
#include <engine/eng_IShape.h>
#include <engine/eng_Interval.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Ray.h>

namespace eng::rt {

class IShader;

class IShape : public eng::IShape {
public:
    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const = 0;

    Handle<IShader> GetShaderRT() const { return static_cast<Handle<IShader>>(this->GetShader()); };
}; // class IShape

} // namespace eng::shape
