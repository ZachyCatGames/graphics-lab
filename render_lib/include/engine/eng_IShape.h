#pragma once
#include <engine/eng_Bounds.h>
#include <engine/eng_Vector3D.h>

namespace eng {

class IShape {
public:
    virtual ~IShape();
    
    virtual Vector3DF GetPosition() const = 0;
    virtual Bounds GetBounds() const = 0;
};

} // namespace eng
