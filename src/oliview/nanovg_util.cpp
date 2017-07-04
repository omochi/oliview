#include "./nanovg_util.h"

namespace oliview {
    void NVGScissor(NVGcontext * ctx,
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
}
