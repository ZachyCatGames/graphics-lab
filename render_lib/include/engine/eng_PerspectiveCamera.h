#pragma once
#include <engine/eng_ICamera.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>
#include <engine/eng_ObjectBase.h>

#ifdef ENGINE_BUILD_GL_RENDER
#include "glm/glm.hpp"
#endif // ENGINE_BUILD_GL_RENDER

namespace eng {

class PerspectiveCamera : public ICamera, public ObjectBase<PerspectiveCamera> {
public:

    virtual ~PerspectiveCamera() = default;

    constexpr void MoveByW(float dist) noexcept { m_position += m_W * dist; }
    constexpr void MoveByU(float dist) noexcept { m_position += m_U * dist;}

#ifdef ENGINE_BUILD_GL_RENDER
    [[nodiscard]] virtual glm::mat4 GetProjectionMatrix(float angleDegrees) const;

    [[nodiscard]] virtual glm::mat4 GetViewMatrix() const;
#endif // ENGINE_BUILD_GL_RENDER
private:

}; // class PerspectiveCamera

} // namespace eng
