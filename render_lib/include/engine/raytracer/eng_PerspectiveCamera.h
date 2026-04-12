#pragma once
#include <engine/eng_CameraBase.h>
#include <engine/eng_ObjectBase.h>
#include <engine/raytracer/eng_ICamera.h>

namespace eng::rt {

class PerspectiveCamera : public CameraBase<ICamera>, public ObjectBase<PerspectiveCamera> {
public:
    using CameraBase::CameraBase;
    virtual ~PerspectiveCamera() = default;

    virtual Ray GenerateRay(float i, float j) override;
}; // class PerspectiveCamera

} // namespace eng::rt
