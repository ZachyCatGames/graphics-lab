#include <engine/gl/eng_Texture.h>

namespace eng::gl {

Texture::Texture(const float* textureData, size_t width, size_t height) :
    eng::Texture(width, height)
{
    /* Generate the texture object. */
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    /* Setup texture parameters. These shouldn't be neccessarily according to the standard but stuff breaks if we don't do it... */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Copy texture data over. */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, textureData);

    /* Unbind the texture object, we're done with it. */
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(GLuint textureUnitId) {
    /* Activate the target texture unit and bind our texture to it. */
    glActiveTexture(textureUnitId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace eng
