#pragma once
#include <engine/eng_Handle.h>
#include <engine/eng_RenderBuffer.h>

namespace eng::gl {

class IRenderable : public RenderBuffer {
public:
    ~IRenderable() override = default;

    virtual void SetContextCurrent() = 0;

    virtual void NotifyRenderDone() = 0;
}; // class IRenderable

inline void SetContextCurrent(const Handle<RenderBuffer>& rb) {
    rb.StaticCast<IRenderable>()->SetContextCurrent();
}

} // namespace eng::gl
