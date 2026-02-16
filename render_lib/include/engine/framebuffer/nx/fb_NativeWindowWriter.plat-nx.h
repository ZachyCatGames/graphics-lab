#pragma once
#include <engine/framebuffer/fb_FramebufferWriterBase.h>
#include <switch.h>

#ifdef __SWITCH__

namespace eng::fb::nx {

class NativeWindowWriter : public FramebufferWriterBase<NativeWindowWriter> {
public:
    NativeWindowWriter(::Framebuffer* fb);
    virtual ~NativeWindowWriter();
private:
    friend class FramebufferWriterBase<NativeWindowWriter>;
    void WriteFramebufferPixelImpl(const Framebuffer& fb, int x, int y);

    void NotifyWriteBegin(int width, int height);
    void NotifyAllPixelsWritten(const Framebuffer&);
private:
    ::Framebuffer* m_framebuffer;
    u32* m_fb_ptr;
    u32 m_stride;
}; // class NativeWindowWriter

} // namespace eng::fb::nx

#else

#error Including NX header on non-NX build

#endif
