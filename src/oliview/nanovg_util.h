#pragma once

#include "./dependency.h"

#include "./color.h"
#include "./font.h"
#include "./matrix3x3.h"
#include "./rect.h"
#include "./string_slice.h"

namespace oliview {
    void NVGSetScissor(NVGcontext * ctx,
                       const Rect & rect);
    void NVGTransform(NVGcontext * ctx,
                      const Matrix3x3 & matrix);
    void NVGAddRectPath(NVGcontext * ctx,
                        const Rect & rect);
    void NVGSetFillColor(NVGcontext * ctx,
                         const Color & color);
    void NVGSetFont(NVGcontext * ctx,
                    const Ptr<Font> & font);
    void NVGDrawText(NVGcontext * ctx,
                     float x,
                     float y,
                     const StringSlice & string);
}

