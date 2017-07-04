#pragma once

#include "./dependency.h"

#include "./color.h"
#include "./matrix3x3.h"
#include "./rect.h"

namespace oliview {
    void NVGScissor(NVGcontext * ctx,
                    const Rect & rect);
    void NVGTransform(NVGcontext * ctx,
                      const Matrix3x3 & matrix);
    void NVGRect(NVGcontext * ctx,
                 const Rect & rect);
    void NVGFillColor(NVGcontext * ctx,
                      const Color & color);
}

