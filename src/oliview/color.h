#pragma once

#include "./dependency.h"

namespace oliview {
    class Color {
    public:
        Color(float red = 0.0f,
              float green = 0.0f,
              float blue = 0.0f,
              float alpha = 1.0f);
        
        RHETORIC_ACCESSOR_TRIVIAL(float, red)
        RHETORIC_ACCESSOR_TRIVIAL(float, green)
        RHETORIC_ACCESSOR_TRIVIAL(float, blue)
        RHETORIC_ACCESSOR_TRIVIAL(float, alpha)

        NVGcolor ToNanoVG() const;
    private:
        float red_;
        float green_;
        float blue_;
        float alpha_;
    };
}
