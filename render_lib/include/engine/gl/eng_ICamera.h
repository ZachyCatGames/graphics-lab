#pragma once
#include <engine/eng_ICamera.h>

#include "glm/glm.hpp"

namespace eng::gl {

class ICamera : public eng::ICamera {
public:
    [[nodiscard]] virtual glm::mat4 GetProjectionMatrix(float angleDegrees) const = 0;

    [[nodiscard]] virtual glm::mat4 GetViewMatrix() const = 0;
}; // class ICamera

} // namespace eng::gl
