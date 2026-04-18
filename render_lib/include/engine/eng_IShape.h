#pragma once
#include <engine/eng_Bounds.h>
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Vector3D.h>

namespace eng {

class IShape {
public:
    virtual ~IShape() = default;
    
    virtual Vector3DF GetPosition() const = 0;
    virtual Bounds GetBounds() const = 0;

    virtual Handle<IShader> GetShader() const = 0;
};

} // namespace eng
