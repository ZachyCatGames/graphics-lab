#pragma once
#include <engine/eng_Texture.h>
#include <GL/glew.h>

namespace eng::gl {

class Texture : public eng::Texture {
public:
    Texture(const float* textureData, size_t width, size_t height);

    void Bind(GLuint textureUnitId);
    void Unbind();
private:
    GLuint m_textureId;
}; // class Texture

} // namespace eng::gl
