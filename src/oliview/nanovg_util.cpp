#include "./nanovg_util.h"

namespace oliview {
    void NVGSetScissor(NVGcontext * ctx,
                       const Rect & rect)
    {
        nvgScissor(ctx,
                   rect.origin().x(),
                   rect.origin().y(),
                   rect.size().width(),
                   rect.size().height());
    }
    
    void NVGTransform(NVGcontext * ctx,
                      const Matrix3x3 & matrix)
    {
        auto m = matrix.Transpose();
        nvgTransform(ctx,
                     m.get(0, 0), m.get(1, 0),
                     m.get(0, 1), m.get(1, 1),
                     m.get(0, 2), m.get(1, 2));
    }
    
    void NVGAddRectPath(NVGcontext * ctx,
                        const Rect & rect)
    {
        nvgRect(ctx,
                rect.origin().x(), rect.origin().y(),
                rect.size().width(), rect.size().height());
    }
    
    void NVGAddRoundRectPath(NVGcontext * ctx,
                             const Rect & rect,
                             float radius)
    {
        nvgRoundedRect(ctx,
                       rect.origin().x(), rect.origin().y(),
                       rect.size().width(), rect.size().height(),
                       radius);
    }
    
    void NVGSetFillColor(NVGcontext * ctx,
                      const Color & color)
    {
        nvgFillColor(ctx, color.ToNanoVG());
    }
    
    void NVGSetFont(NVGcontext * ctx,
                    const Ptr<Font> & font)
    {
        nvgFontFaceId(ctx, font->nvg_handle());
    }
    
    void NVGDrawText(NVGcontext * ctx,
                     float x,
                     float y,
                     const std::string & string)
    {
        nvgText(ctx, x, y,
                string.c_str(),
                string.c_str() + string.length());
    }
}
