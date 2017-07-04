#pragma once

#include "./dependency.h"

#include "./matrix3x3.h"
#include "./rect.h"

namespace oliview {
    void NVGScissor(NVGcontext * ctx,
                    const Rect & rect);
    void NVGTransform(NVGcontext * ctx,
                      const Matrix3x3 & matrix);
}
