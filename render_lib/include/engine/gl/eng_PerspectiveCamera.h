#pragma once
#include <cmath>
#include <engine/eng_CameraBase.h>
#include <engine/eng_ObjectBase.h>
#include <engine/gl/eng_ICamera.h>
#include <glm/trigonometric.hpp>

namespace eng::gl {

class PerspectiveCamera : public CameraBase<ICamera>, public ObjectBase<PerspectiveCamera> {
public:
    using CameraBase::CameraBase;

    [[nodiscard]] virtual glm::mat4 GetProjectionMatrix(float angleDegrees) const override;

    [[nodiscard]] virtual glm::mat4 GetViewMatrix() const override;
}; // class PerspectiveCamera

} // namespace eng::gl
