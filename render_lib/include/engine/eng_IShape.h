#pragma once
#include <engine/eng_Bounds.h>
#include <engine/eng_Handle.h>
#include <engine/eng_IShader.h>
#include <engine/eng_Material.h>
#include <engine/eng_Vector3D.h>
#include <numbers>

namespace eng {

class IShape {
public:
    IShape() : rotationAxis(0, 1, 0), rotationAngle(0) {}
    virtual ~IShape() = default;
    
    virtual Vector3DF GetPosition() const = 0;
    //virtual void SetPosition(Vector3DF position);

    virtual Bounds GetBounds() const = 0;

    virtual Handle<IShader> GetShader() const = 0;
public:
    void IncrementRotationAngle(float amount) {
        static constexpr Interval<float> angleRange(0, 2 * std::numbers::pi);
        rotationAngle += amount;
        if (!angleRange.Contains(rotationAngle)) {
            rotationAngle = std::fmodf(rotationAngle, 2 * std::numbers::pi);
        }
    }
public:
    Vector3DF rotationAxis;
    float rotationAngle;
};

} // namespace eng
