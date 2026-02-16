#pragma once
#include "IFramebufferWriter.h"

namespace eng {

template<typename DerivedT>
class FramebufferWriterBase : public IFramebufferWriter {
public:
    virtual void WriteFramebuffer(const Framebuffer& fb) override {
        auto [width, height] = fb.GetDimensions();

        if constexpr (requires { GetDerived()->NotifyWriteBegin(width, height); })
            GetDerived()->NotifyWriteBegin(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                GetDerived()->WriteFramebufferPixelImpl(fb, x, y);
            }
        }

        if constexpr (requires { GetDerived()->NotifyAllPixelsWritten(fb); })
            GetDerived()->NotifyAllPixelsWritten(fb);
    }
private:
    auto GetDerived() { return static_cast<DerivedT*>(this); }
    const auto GetDerived() const { return static_cast<const DerivedT*>(this); }
}; // class FramebufferWriterBase

} // namespace eng
