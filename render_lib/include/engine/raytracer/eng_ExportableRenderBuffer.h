#pragma once
#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/raytracer/eng_IDrawable.h>
#include <engine/eng_ExportableRenderBuffer.h>
#include <engine/eng_ObjectBase.h>

namespace eng::rt {

class ExportableRenderBuffer : public eng::ExportableRenderBuffer, public IDrawable {
public:
    ~ExportableRenderBuffer() override = default;

    ExportableRenderBuffer(size_t width, size_t height);

    void SetPixelColor(size_t x, size_t y, Vector3DF color) override;

    [[nodiscard]] fb::Framebuffer ExportToFramebuffer() const override;

    [[nodiscard]] size_t GetWidth() const override;
    [[nodiscard]] size_t GetHeight() const override;
private:
    fb::Framebuffer m_framebuffer;
}; // class ExportableRenderBuffer

} // namespace eng::rt
