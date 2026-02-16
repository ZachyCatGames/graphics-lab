#pragma once
#include <engine/framebuffer/fb_Framebuffer.h>

namespace eng::fb {

class IFramebufferWriter {
public:
    virtual void WriteFramebuffer(const Framebuffer& fb) = 0;
}; // class IFramebufferWriter

} // namespace eng::fb
