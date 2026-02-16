#pragma once
#include "Framebuffer.h"

namespace eng {

class IFramebufferWriter {
public:
    virtual void WriteFramebuffer(const Framebuffer& fb) = 0;
}; // class IFramebufferWriter

} // namespace eng
